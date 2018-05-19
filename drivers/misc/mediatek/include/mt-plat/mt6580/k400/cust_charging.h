// #ifndef _CUST_BAT_H_
// #define _CUST_BAT_H_

/* stop charging while in talking mode */
#define STOP_CHARGING_IN_TAKLING
#define TALKING_RECHARGE_VOLTAGE 3800
#define TALKING_SYNC_TIME		   60

/* Battery Temperature Protection */
#define MTK_TEMPERATURE_RECHARGE_SUPPORT
#define MAX_CHARGE_TEMPERATURE  61
#define MAX_CHARGE_TEMPERATURE_MINUS_X_DEGREE	58
#define MIN_CHARGE_TEMPERATURE  -5
#define MIN_CHARGE_TEMPERATURE_PLUS_X_DEGREE	0
#define ERR_CHARGE_TEMPERATURE  0xFF
/* Linear Charging Threshold */
#define V_PRE2CC_THRES 3400
#define V_CC2TOPOFF_THRES		4050
#define RECHARGING_VOLTAGE      4110
#define CHARGING_FULL_CURRENT    200  // 100

/* Charging Current Setting */
#define USB_CHARGER_CURRENT_SUSPEND			0
#define USB_CHARGER_CURRENT_UNCONFIGURED	CHARGE_CURRENT_70_00_MA
#define USB_CHARGER_CURRENT_CONFIGURED		CHARGE_CURRENT_500_00_MA

#define USB_CHARGER_CURRENT					CHARGE_CURRENT_500_00_MA
#define USB_INPUT_CHARGER_CURRENT			CHARGE_CURRENT_500_00_MA


#ifdef TINNO_2A_CHARGER //Tinno 2A Charger

#define AC_45_TO_60_TEMP_CHARGER_CURRENT		CHARGE_CURRENT_1000_00_MA
#define AC_45_TO_60_TEMP_INPUT_CHARGER_CURRENT	CHARGE_CURRENT_1000_00_MA
#define AC_CHARGER_CURRENT						CHARGE_CURRENT_1200_00_MA  //temp 25 current
#define AC_INPUT_CHARGER_CURRENT				CHARGE_CURRENT_2000_00_MA  //temp 25 input current
#define AC_0_TO_10_TEMP_CHARGER_CURRENT			CHARGE_CURRENT_550_00_MA
#define AC_0_TO_10_TEMP_INPUT_CHARGER_CURRENT	CHARGE_CURRENT_550_00_MA

#else //TINNO 1.5A CHARGER

#define AC_45_TO_60_TEMP_CHARGER_CURRENT		CHARGE_CURRENT_650_00_MA
#define AC_45_TO_60_TEMP_INPUT_CHARGER_CURRENT	CHARGE_CURRENT_800_00_MA
#define AC_CHARGER_CURRENT						CHARGE_CURRENT_950_00_MA  //temp 25 current
#define AC_INPUT_CHARGER_CURRENT				CHARGE_CURRENT_1100_00_MA  //temp 25 input current
#define AC_0_TO_10_TEMP_CHARGER_CURRENT			CHARGE_CURRENT_550_00_MA
#define AC_0_TO_10_TEMP_INPUT_CHARGER_CURRENT	CHARGE_CURRENT_800_00_MA
#endif

#define NON_STD_AC_45_TO_60_TEMP_CHARGER_CURRENT		CHARGE_CURRENT_500_00_MA
#define NON_STD_AC_45_TO_60_TEMP_INPUT_CHARGER_CURRENT	CHARGE_CURRENT_800_00_MA
#define NON_STD_AC_CHARGER_CURRENT				CHARGE_CURRENT_500_00_MA  //temp 25 current
#define NON_STD_AC_INPUT_CHARGER_CURRENT				CHARGE_CURRENT_1100_00_MA  //temp 25 input current
#define NON_STD_AC_0_TO_10_TEMP_CHARGER_CURRENT			CHARGE_CURRENT_500_00_MA
#define NON_STD_AC_0_TO_10_TEMP_INPUT_CHARGER_CURRENT	CHARGE_CURRENT_800_00_MA


#define CHARGING_HOST_CHARGER_CURRENT       CHARGE_CURRENT_550_00_MA
#define APPLE_0_5A_CHARGER_CURRENT          CHARGE_CURRENT_500_00_MA
#define APPLE_1_0A_CHARGER_CURRENT          CHARGE_CURRENT_975_00_MA

#define APPLE_2_1A_45_TO_60_TEMP_CHARGER_CURRENT        CHARGE_CURRENT_1000_00_MA
#define APPLE_2_1A_45_TO_60_TEMP_INPUT_CHARGER_CURRENT  CHARGE_CURRENT_1000_00_MA
#define APPLE_2_1A_CHARGER_CURRENT          	  		CHARGE_CURRENT_950_00_MA  //temp 25 current
#define APPLE_2_1A_INPUT_CHARGER_CURRENT          		CHARGE_CURRENT_1800_00_MA  //temp 25 input current
#define APPLE_2_1A_0_TO_10_TEMP_CHARGER_CURRENT         CHARGE_CURRENT_550_00_MA
#define APPLE_2_1A_0_TO_10_TEMP_INPUT_CHARGER_CURRENT   CHARGE_CURRENT_550_00_MA


#define JEITA_TEMP_POS_45_TO_POS_60_CC_CURRENT	CHARGE_CURRENT_650_00_MA	
#define JEITA_TEMP_POS_10_TO_POS_45_CC_CURRENT	CHARGE_CURRENT_950_00_MA
#define JEITA_TEMP_POS_0_TO_POS_10_CC_CURRENT	CHARGE_CURRENT_550_00_MA 

/* Precise Tunning */
#define BATTERY_AVERAGE_DATA_NUMBER	3	
#define BATTERY_AVERAGE_SIZE 	15

/* charger error check */
//#define BAT_LOW_TEMP_PROTECT_ENABLE         // stop charging if temp < MIN_CHARGE_TEMPERATURE
#define V_CHARGER_ENABLE 0				// 1:ON , 0:OFF	
#define V_CHARGER_MAX 6500				// 6.5 V
#define V_CHARGER_MIN 4400				// 4.4 V

/* Tracking TIME */
#define ONEHUNDRED_PERCENT_TRACKING_TIME	10	// 10 second
#define NPERCENT_TRACKING_TIME	   			20	// 20 second
#define SYNC_TO_REAL_TRACKING_TIME  		60	// 60 second
#define V_0PERCENT_TRACKING							3450 //3450mV

/* Battery Notify */
#define BATTERY_NOTIFY_CASE_0001_VCHARGER
#define BATTERY_NOTIFY_CASE_0002_VBATTEMP
//#define BATTERY_NOTIFY_CASE_0003_ICHARGING
//#define BATTERY_NOTIFY_CASE_0004_VBAT
//#define BATTERY_NOTIFY_CASE_0005_TOTAL_CHARGINGTIME

/* High battery support */
#define HIGH_BATTERY_VOLTAGE_SUPPORT

/* JEITA parameter */
//#define MTK_JEITA_STANDARD_SUPPORT
#define CUST_SOC_JEITA_SYNC_TIME 30
#define JEITA_RECHARGE_VOLTAGE  4200	// for linear charging

#ifdef HIGH_BATTERY_VOLTAGE_SUPPORT
#define JEITA_TEMP_ABOVE_POS_60_CV_VOLTAGE		BATTERY_VOLT_04_100000_V
#define JEITA_TEMP_POS_45_TO_POS_60_CV_VOLTAGE		BATTERY_VOLT_04_100000_V
#define JEITA_TEMP_POS_10_TO_POS_45_CV_VOLTAGE		BATTERY_VOLT_04_340000_V
#define JEITA_TEMP_POS_0_TO_POS_10_CV_VOLTAGE		BATTERY_VOLT_04_340000_V
#define JEITA_TEMP_NEG_10_TO_POS_0_CV_VOLTAGE		BATTERY_VOLT_04_300000_V
#define JEITA_TEMP_BELOW_NEG_10_CV_VOLTAGE		BATTERY_VOLT_04_300000_V
#else
#define JEITA_TEMP_ABOVE_POS_60_CV_VOLTAGE		BATTERY_VOLT_04_000000_V
#define JEITA_TEMP_POS_45_TO_POS_60_CV_VOLTAGE	BATTERY_VOLT_04_000000_V
#define JEITA_TEMP_POS_10_TO_POS_45_CV_VOLTAGE	BATTERY_VOLT_04_200000_V
#define JEITA_TEMP_POS_0_TO_POS_10_CV_VOLTAGE	BATTERY_VOLT_04_100000_V
#define JEITA_TEMP_NEG_10_TO_POS_0_CV_VOLTAGE	BATTERY_VOLT_03_900000_V
#define JEITA_TEMP_BELOW_NEG_10_CV_VOLTAGE		BATTERY_VOLT_03_900000_V
#endif
/* For JEITA Linear Charging only */
#define JEITA_NEG_10_TO_POS_0_FULL_CURRENT  120	//mA 
#define JEITA_TEMP_POS_45_TO_POS_60_RECHARGE_VOLTAGE  3800
#define JEITA_TEMP_POS_10_TO_POS_45_RECHARGE_VOLTAGE  4250 //4100
#define JEITA_TEMP_POS_0_TO_POS_10_RECHARGE_VOLTAGE   4000
#define JEITA_TEMP_NEG_10_TO_POS_0_RECHARGE_VOLTAGE   4000

#define JEITA_TEMP_POS_45_TO_POS_60_CC2TOPOFF_THRESHOLD	4100
#define JEITA_TEMP_POS_10_TO_POS_45_CC2TOPOFF_THRESHOLD	4340
#define JEITA_TEMP_POS_0_TO_POS_10_CC2TOPOFF_THRESHOLD	4300
#define JEITA_TEMP_NEG_10_TO_POS_0_CC2TOPOFF_THRESHOLD	4300


/* High battery support */
//#define HIGH_BATTERY_VOLTAGE_SUPPORT

/* Disable Battery check for HQA */
#ifdef CONFIG_MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
#define CONFIG_DIS_CHECK_BATTERY
#endif

#ifdef CONFIG_MTK_FAN5405_SUPPORT
#define FAN5405_BUSNUM 1
#endif

// #endif /* _CUST_BAT_H_ */ 
