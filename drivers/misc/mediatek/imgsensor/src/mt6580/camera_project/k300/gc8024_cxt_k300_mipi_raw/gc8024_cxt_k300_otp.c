#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>


#include "kd_camera_hw.h"
#include "kd_imgsensor.h" 
#include "kd_imgsensor_define.h"
#include "kd_imgsensor_errcode.h"
#include "cam_cal.h"
#include "cam_cal_define.h"

#include "gc8024_cxt_k300_mipi_Sensor.h"

#define PFX "gc8024_cxt_k300_otp"
#define LOG_INF(format, args...)    pr_debug(PFX "[%s] " format, __FUNCTION__, ##args)

static DEFINE_SPINLOCK(g_CAM_CALLock); // for SMP
#define CAM_CAL_I2C_BUSNUM 0
#define GC8024_CXT_K300_DEVICE_ID 0x20
#define CAM_CAL_DEV_MAJOR_NUMBER 226

/*******************************************************************************
 *
 ********************************************************************************/
#define CAM_CAL_ICS_REVISION 1 //seanlin111208
/*******************************************************************************
 *
 ********************************************************************************/
#define CAM_CAL_DRVNAME "GC8024_CXT_K300_CAM_CAL_DRV"
//#define CAM_CAL_DRVNAME "CAM_CAL_DRV"
#define CAM_CAL_I2C_GROUP_ID 0
/*******************************************************************************
 *
 ********************************************************************************/
/* fix warning MSG 
   static unsigned short g_pu2Normal_i2c[] = {GC8024_CXT_K300__DEVICE_ID , I2C_CLIENT_END};
   static unsigned short g_u2Ignore = I2C_CLIENT_END;
   static struct i2c_client_address_data g_stCAM_CAL_Addr_data = {
   .normal_i2c = g_pu2Normal_i2c,
   .probe = &g_u2Ignore,
   .ignore = &g_u2Ignore
   }; */

static struct i2c_board_info __initdata kd_cam_cal_dev={ I2C_BOARD_INFO(CAM_CAL_DRVNAME, 0x20>>1)}; //make dummy_eeprom co-exist

//81 is used for V4L driver
static dev_t g_CAM_CALdevno = MKDEV(CAM_CAL_DEV_MAJOR_NUMBER,0);
static struct cdev * g_pCAM_CAL_CharDrv = NULL;
//static spinlock_t g_CAM_CALLock;
static struct class *CAM_CAL_class = NULL;
static atomic_t g_CAM_CALatomic;
/*******************************************************************************
 *
 ********************************************************************************/

u8 GC8024_CXT_K300_CheckID[]= {0x10,0xbc,0x00,0x40,0x88};
extern u16 otp_gc8024_cxt_k300_af_data[2];

static int selective_read_region(u32 offset, BYTE* data,u16 i2c_id,u32 size)
{    	
	printk("[GC8024_CXT_K300__CAM_CAL] selective_read_region offset =%d size %d data read = %d\n", offset,size, *data);

	if(size ==2 ){
		if(offset == 7){
			memcpy((void *)data,(void *)&otp_gc8024_cxt_k300_af_data[0],size);
			printk("otp_gc8024cxt_k300_af_data[0] = 0x%x\n", otp_gc8024_cxt_k300_af_data[0]);

		}

		else if(offset == 9){		 
			memcpy((void *)data,(void *)&otp_gc8024_cxt_k300_af_data[1],size);
			printk("otp_gc8024cxt_k300_af_data[1] = 0x%x\n",otp_gc8024_cxt_k300_af_data[1]);

		}
	}

	if(size == 4){
		memcpy((void *)data,(void *)&GC8024_CXT_K300_CheckID[1],size);

	}
	printk("+ls.test[GC8024_CXT_K300__CAM_CAL]  data1 = %d\n",*(UINT32 *)data);
}

#define NEW_UNLOCK_IOCTL
#ifndef NEW_UNLOCK_IOCTL
static int CAM_CAL_Ioctl(struct inode * a_pstInode,
		struct file * a_pstFile,
		unsigned int a_u4Command,
		unsigned long a_u4Param)
#else 
static long CAM_CAL_Ioctl(
		struct file *file, 
		unsigned int a_u4Command, 
		unsigned long a_u4Param
		)
#endif
{
	int i4RetValue = 0;
	u8 * pBuff = NULL;
	u8 * pWorkingBuff = NULL;
	stCAM_CAL_INFO_STRUCT *ptempbuf;

#ifdef CAM_CALGETDLT_DEBUG
	struct timeval ktv1, ktv2;
	unsigned long TimeIntervalUS;
#endif

	if(_IOC_NONE == _IOC_DIR(a_u4Command))
	{
	}
	else
	{
		pBuff = (u8 *)kmalloc(sizeof(stCAM_CAL_INFO_STRUCT),GFP_KERNEL);

		if(NULL == pBuff)
		{
			LOG_INF("[GC8024_CXT_K300__CAM_CAL] ioctl allocate mem failed\n");
			return -ENOMEM;
		}

		if(_IOC_WRITE & _IOC_DIR(a_u4Command))
		{
			if(copy_from_user((u8 *) pBuff , (u8 *) a_u4Param, sizeof(stCAM_CAL_INFO_STRUCT)))
			{    //get input structure address
				kfree(pBuff);
				LOG_INF("[GC8024_CXT_K300__CAM_CAL] ioctl copy from user failed\n");
				return -EFAULT;
			}
		}
	}

	ptempbuf = (stCAM_CAL_INFO_STRUCT *)pBuff;
	pWorkingBuff = (u8*)kmalloc(ptempbuf->u4Length,GFP_KERNEL); 
	if(NULL == pWorkingBuff)
	{
		kfree(pBuff);
		LOG_INF("[GC8024_CXT_K300__CAM_CAL] ioctl allocate mem failed\n");
		return -ENOMEM;
	}
	//fix warning MSG     LOG_INF("[GC8024_CXT_K300__CAM_CAL] init Working buffer address 0x%x  command is 0x%08x\n", pWorkingBuff, a_u4Command);


	if(copy_from_user((u8*)pWorkingBuff ,  (u8*)ptempbuf->pu1Params, ptempbuf->u4Length))
	{
		kfree(pBuff);
		kfree(pWorkingBuff);
		LOG_INF("[GC8024_CXT_K300__CAM_CAL] ioctl copy from user failed\n");
		return -EFAULT;
	} 

	switch(a_u4Command)
	{


		case CAM_CALIOC_S_WRITE:
			LOG_INF("[LOG_INF] CAM_CALIOC_S_WRITE \n");        
			#ifdef CAM_CALGETDLT_DEBUG
			do_gettimeofday(&ktv1);
			#endif
			i4RetValue = 0;
			// i4RetValue=iWriteData((u16)ptempbuf->u4Offset, ptempbuf->u4Length, pu1Params);
			#ifdef CAM_CALGETDLT_DEBUG
			do_gettimeofday(&ktv2);
			if(ktv2.tv_sec > ktv1.tv_sec)
			{
				TimeIntervalUS = ktv1.tv_usec + 1000000 - ktv2.tv_usec;
			}
			else
			{
				TimeIntervalUS = ktv2.tv_usec - ktv1.tv_usec;
			}
			#endif
			break;



		case CAM_CALIOC_G_READ:
			LOG_INF("[GC8024_CXT_K300__CAM_CAL] Read CMD \n");
			#ifdef CAM_CALGETDLT_DEBUG            
			do_gettimeofday(&ktv1);
			#endif 
			LOG_INF("[GC8024_CXT_K300__CAM_CAL] offset %d \n", ptempbuf->u4Offset);
			LOG_INF("[GC8024_CXT_K300__CAM_CAL] length %d \n", ptempbuf->u4Length);
			// LOG_INF("[GC8024_CXT_K300__CAM_CAL] Before read Working buffer address 0x%x \n", pWorkingBuff);


			if(ptempbuf->u4Length == 2){

				i4RetValue = selective_read_region(ptempbuf->u4Offset, pWorkingBuff, 0x20, ptempbuf->u4Length);

			}else if(ptempbuf->u4Length == 4){

				i4RetValue = selective_read_region(ptempbuf->u4Offset, pWorkingBuff, 0x20, ptempbuf->u4Length);
			}        

			// LOG_INF("[GC8024_CXT_K300__CAM_CAL] After read Working buffer address 0x%x \n", pWorkingBuff);


			#ifdef CAM_CALGETDLT_DEBUG
			do_gettimeofday(&ktv2);
			if(ktv2.tv_sec > ktv1.tv_sec)
			{
				TimeIntervalUS = ktv1.tv_usec + 1000000 - ktv2.tv_usec;
			}
			else
			{
				TimeIntervalUS = ktv2.tv_usec - ktv1.tv_usec;
			}
			printk("Read data %d bytes take %lu us\n",ptempbuf->u4Length, TimeIntervalUS);
			#endif            

			break;
		default :
			LOG_INF("[GC8024_CXT_K300__CAM_CAL] No CMD \n");
			i4RetValue = -EPERM;
			break;
	}

	if(_IOC_READ & _IOC_DIR(a_u4Command))
	{
		//copy data to user space buffer, keep other input paremeter unchange.
		LOG_INF("[GC8024_CXT_K300__CAM_CAL] to user length %d \n", ptempbuf->u4Length);




		if(copy_to_user((u8 __user *) ptempbuf->pu1Params , (u8 *)pWorkingBuff , ptempbuf->u4Length))
		{
			kfree(pBuff);
			kfree(pWorkingBuff);
			LOG_INF("[GC8024_CXT_K300__CAM_CAL] ioctl copy to user failed\n");
			return -EFAULT;
		}
	}

	kfree(pBuff);
	kfree(pWorkingBuff);
	return i4RetValue;
}


static u32 g_u4Opened = 0;
//#define
//Main jobs:
// 1.check for device-specified errors, device not ready.
// 2.Initialize the device if it is opened for the first time.
static int CAM_CAL_Open(struct inode * a_pstInode, struct file * a_pstFile)
{
	LOG_INF("[GC8024_CXT_K300_ CAM_CAL] CAM_CAL_Open\n");
	spin_lock(&g_CAM_CALLock);
	if(g_u4Opened)
	{
		spin_unlock(&g_CAM_CALLock);
		return -EBUSY;
	}
	else
	{
		g_u4Opened = 1;
		atomic_set(&g_CAM_CALatomic,0);
	}
	spin_unlock(&g_CAM_CALLock);

	//#if defined(MT6572)
	// do nothing
	//#else
	//if(TRUE != hwPowerOn(MT65XX_POWER_LDO_VCAMA, VOL_2800, "S24CS64A"))
	//{
	//    LOG_INF("[GC8024_CXT_K300__CAM_CAL] Fail to enable analog gain\n");
	//    return -EIO;
	//}
	//#endif	

	return 0;
}

//Main jobs:
// 1.Deallocate anything that "open" allocated in private_data.
// 2.Shut down the device on last close.
// 3.Only called once on last time.
// Q1 : Try release multiple times.
static int CAM_CAL_Release(struct inode * a_pstInode, struct file * a_pstFile)
{
	spin_lock(&g_CAM_CALLock);

	g_u4Opened = 0;

	atomic_set(&g_CAM_CALatomic,0);

	spin_unlock(&g_CAM_CALLock);

	return 0;
}

static const struct file_operations g_stCAM_CAL_fops =
{
	.owner = THIS_MODULE,
	.open = CAM_CAL_Open,
	.release = CAM_CAL_Release,
	//.ioctl = CAM_CAL_Ioctl
	.unlocked_ioctl = CAM_CAL_Ioctl
};

#define CAM_CAL_DYNAMIC_ALLOCATE_DEVNO 1
inline static int RegisterCAM_CALCharDrv(void)
{
	struct device* CAM_CAL_device = NULL;

#if CAM_CAL_DYNAMIC_ALLOCATE_DEVNO
	if( alloc_chrdev_region(&g_CAM_CALdevno, 0, 1,CAM_CAL_DRVNAME) )
	{
		LOG_INF("[GC8024_CXT_K300__CAM_CAL] Allocate device no failed\n");

		return -EAGAIN;
	}
#else
	if( register_chrdev_region(  g_CAM_CALdevno , 1 , CAM_CAL_DRVNAME) )
	{
		LOG_INF("[GC8024_CXT_K300__CAM_CAL] Register device no failed\n");

		return -EAGAIN;
	}
#endif

	//Allocate driver
	g_pCAM_CAL_CharDrv = cdev_alloc();

	if(NULL == g_pCAM_CAL_CharDrv)
	{
		unregister_chrdev_region(g_CAM_CALdevno, 1);

		LOG_INF("[GC8024_CXT_K300__CAM_CAL] Allocate mem for kobject failed\n");

		return -ENOMEM;
	}

	//Attatch file operation.
	cdev_init(g_pCAM_CAL_CharDrv, &g_stCAM_CAL_fops);

	g_pCAM_CAL_CharDrv->owner = THIS_MODULE;

	//Add to system
	if(cdev_add(g_pCAM_CAL_CharDrv, g_CAM_CALdevno, 1))
	{
		LOG_INF("[GC8024_CXT_K300__CAM_CAL] Attatch file operation failed\n");

		unregister_chrdev_region(g_CAM_CALdevno, 1);

		return -EAGAIN;
	}

	CAM_CAL_class = class_create(THIS_MODULE, "GC8024_CXT_K300__CAM_CALdrv");
	if (IS_ERR(CAM_CAL_class)) {
		int ret = PTR_ERR(CAM_CAL_class);
		LOG_INF("Unable to create class, err = %d\n", ret);
		return ret;
	}

	CAM_CAL_device = device_create(CAM_CAL_class, NULL, g_CAM_CALdevno, NULL, CAM_CAL_DRVNAME);

	return 0;
}

inline static void UnregisterCAM_CALCharDrv(void)
{
	//Release char driver
	cdev_del(g_pCAM_CAL_CharDrv);

	unregister_chrdev_region(g_CAM_CALdevno, 1);

	device_destroy(CAM_CAL_class, g_CAM_CALdevno);
	class_destroy(CAM_CAL_class);
}

static int CAM_CAL_probe(struct platform_device *pdev)
{

	return 0;//i2c_add_driver(&CAM_CAL_i2c_driver);
}

static int CAM_CAL_remove(struct platform_device *pdev)
{
	//i2c_del_driver(&CAM_CAL_i2c_driver);
	return 0;
}

// platform structure
static struct platform_driver g_stCAM_CAL_Driver = {
	.probe		= CAM_CAL_probe,
	.remove	= CAM_CAL_remove,
	.driver		= {
		.name	= CAM_CAL_DRVNAME,
		.owner	= THIS_MODULE,
	}
};


static struct platform_device g_stCAM_CAL_Device = {
	.name = CAM_CAL_DRVNAME,
	.id = 0,
	.dev = {
	}
};

static int __init CAM_CAL_i2C_init(void)
{

	// i2c_register_board_info(CAM_CAL_I2C_BUSNUM, &kd_cam_cal_dev, 1);


	int i4RetValue = 0;
	LOG_INF("[GC8024_CXT_K300__CAM_CAL]\n");
	//Register char driver
	i4RetValue = RegisterCAM_CALCharDrv();
	if(i4RetValue){
		LOG_INF(" [GC8024_CXT_K300__CAM_CAL] register char device failed!\n");
		return i4RetValue;
	}
	LOG_INF(" [GC8024_CXT_K300__CAM_CAL] Attached!! \n");


	if(platform_driver_register(&g_stCAM_CAL_Driver)){
		printk("failed to register CAM_CAL driver\n");
		return -ENODEV;
	}
	if (platform_device_register(&g_stCAM_CAL_Device))
	{
		printk("failed to register CAM_CAL driver\n");
		return -ENODEV;
	}	
	LOG_INF(" GC8024_CXT_K300__CAM_CAL  Attached Pass !! \n");
	return 0;
}

static void __exit CAM_CAL_i2C_exit(void)
{
	platform_driver_unregister(&g_stCAM_CAL_Driver);
}

module_init(CAM_CAL_i2C_init);
module_exit(CAM_CAL_i2C_exit);
