#pragma once

#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "global_struct.h"
#include "OutPutDlg.h"
// CPressureSensorForm form view

class CPressureSensorForm : public CFormView
{
	DECLARE_DYNCREATE(CPressureSensorForm)

public:
	CPressureSensorForm();           // protected constructor used by dynamic creation
	virtual ~CPressureSensorForm();
	friend class COutPutDlg;
public:
	enum { IDD = IDD_PRESSURE_SENSOR };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
int m_sn;
 vector<T3Register> m_vecT3Register;
void Fresh();
void Initial_RegisterList();
void Show_Pressure();
int  Get_RegID(CString Name);
	DECLARE_MESSAGE_MAP()
	virtual void OnInitialUpdate();
	CString Get_PressureSensor(double sensor_value);
	void Changing_Parter();
	HANDLE hFirstThread;
UINT m_EDIT_ID;
private:
	int	MODBUS_SERIALNUMBER_LOWORD	;
	int	MODBUS_SERIALNUMBER_HIWORD	;
	int	MODBUS_VERSION_NUMBER_LO	;
	int	MODBUS_VERSION_NUMBER_HI	;
	int	MODBUS_ADDRESS	;
	int	MODBUS_PRODUCT_MODEL	;
	int	MODBUS_HARDWARE_REV	;
	int	MODBUS_PIC_VERSION	;
	int	MODBUS_ADDRESS_PLUG_N_PLAY	;
	int	MODBUS_CUSTOM_ADDRESS	;
	int	MODBUS_BASE_ADDRESS	;
	int	MODBUS_UPDATE_STATUS	;
	int	MODBUS_UPDATE_PTR_HI	;
	int	MODBUS_UPDATE_PTR_LO	;
	int	MODBUS_SCRATCHPAD_ADDRESS	;
	int	MODBUS_TEMPRATURE_CHIP	;
	int	MODBUS_COOLING_VALVE	;
	int	MODBUS_HEATING_VALVE	;
	int	MODBUS_COOLING_PID	;
	int	MODBUS_HEATING_PID	;
	int	MODBUS_COOL_HEAT_MODE	;
	int	MODBUS_MODE_OPERATION	;
	int	MODBUS_DIGITAL_OUTPUT_STATUS	;
	int	MODBUS_CALIBRATION	;
	int	MODBUS_CALIBRATION_ANALOG_IN1	;
	int	MODBUS_TEMP_SELECT	;
	int	MODBUS_DAC_OFFSET	;
	int	MODBUS_DELAY	;
	int	MODBUS_COOLING_PTERM	;
	int	MODBUS_COOLING_ITERM	;
	int	MODBUS_HEATING_PTERM	;
	int	MODBUS_HEATING_ITERM	;
	int	MODBUS_SEQUENCE	;
	int	MODBUS_COOLING_DEADBAND	;
	int	MODBUS_HEATING_DEADBAND	;
	int	MODBUS_DEGC_OR_F	;
	int	MODBUS_FAN_MODE	;
	int	MODBUS_NIGHT_HEATING_DEADBAND	;
	int	MODBUS_NIGHT_COOLING_DEADBAND	;
	int	MODBUS_APPLICATION	;
	int	MODBUS_POWERUP_SETPOINT	;
	int	MODBUS_POWERUP_MODE	;
	int	MODBUS_KEYPAD_SELECT	;
	int	MODBUS_AUTO_ONLY	;
	int	MODBUS_OUTPUT_SCALE	;
	int	MODBUS_MAX_SETPOINT	;
	int	MODBUS_MIN_SETPOINT	;
	int	MODBUS_SPECIAL_MENU_LOCK	;
	int	MODBUS_FACTORY_DEFAULTS	;
	int	MODBUS_COOLING_SETPOINT	;
	int	MODBUS_HEATING_SETPOINT	;
	int	MODBUS_FAN_SPEED	;
	int	MODBUS_FAN0_OPER_TABLE_COAST	;
	int	MODBUS_FAN0_OPER_TABLE_COOL1	;
	int	MODBUS_FAN0_OPER_TABLE_COOL2	;
	int	MODBUS_FAN0_OPER_TABLE_COOL3	;
	int	MODBUS_FAN0_OPER_TABLE_HEAT1	;
	int	MODBUS_FAN0_OPER_TABLE_HEAT2	;
	int	MODBUS_FAN0_OPER_TABLE_HEAT3	;
	int	MODBUS_FAN1_OPER_TABLE_COAST	;
	int	MODBUS_FAN1_OPER_TABLE_COOL1	;
	int	MODBUS_FAN1_OPER_TABLE_COOL2	;
	int	MODBUS_FAN1_OPER_TABLE_COOL3	;
	int	MODBUS_FAN1_OPER_TABLE_HEAT1	;
	int	MODBUS_FAN1_OPER_TABLE_HEAT2	;
	int	MODBUS_FAN1_OPER_TABLE_HEAT3	;
	int	MODBUS_FAN2_OPER_TABLE_COAST	;
	int	MODBUS_FAN2_OPER_TABLE_COOL1	;
	int	MODBUS_FAN2_OPER_TABLE_COOL2	;
	int	MODBUS_FAN2_OPER_TABLE_COOL3	;
	int	MODBUS_FAN2_OPER_TABLE_HEAT1	;
	int	MODBUS_FAN2_OPER_TABLE_HEAT2	;
	int	MODBUS_FAN2_OPER_TABLE_HEAT3	;
	int	MODBUS_FAN3_OPER_TABLE_COAST	;
	int	MODBUS_FAN3_OPER_TABLE_COOL1	;
	int	MODBUS_FAN3_OPER_TABLE_COOL2	;
	int	MODBUS_FAN3_OPER_TABLE_COOL3	;
	int	MODBUS_FAN3_OPER_TABLE_HEAT1	;
	int	MODBUS_FAN3_OPER_TABLE_HEAT2	;
	int	MODBUS_FAN3_OPER_TABLE_HEAT3	;
	int	MODBUS_FANAUT_OPER_TABLE_COAST	;
	int	MODBUS_FANAUT_OPER_TABLE_COOL1	;
	int	MODBUS_FANAUT_OPER_TABLE_COOL2	;
	int	MODBUS_FANAUT_OPER_TABLE_COOL3	;
	int	MODBUS_FANAUT_OPER_TABLE_HEAT1	;
	int	MODBUS_FANAUT_OPER_TABLE_HEAT2	;
	int	MODBUS_FANAUT_OPER_TABLE_HEAT3	;
	int	MODBUS_VALVE_OPERATION_TABLE_BEGIN	;
	int	MODBUS_VALVE_OPER_TABLE_COOL1	;
	int	MODBUS_VALVE_OPER_TABLE_COOL2	;
	int	MODBUS_VALVE_OPER_TABLE_COOL3	;
	int	MODBUS_VALVE_OPER_TABLE_HEAT1	;
	int	MODBUS_VALVE_OPER_TABLE_HEAT2	;
	int	MODBUS_VALVE_OPER_TABLE_HEAT3	;
	int	MODBUS_EXTERNAL_SENSOR_0	;
	int	MODBUS_EXTERNAL_SENSOR_1	;
	int	MODBUS_NIGHT_HEATING_SETPOINT	;
	int	MODBUS_NIGHT_COOLING_SETPOINT	;
	int	MODBUS_INFO_BYTE	;
	int	MODBUS_BAUDRATE	;
	int	MODBUS_OUTPUT1_SCALE	;
	int	MODBUS_OUTPUT2_SCALE	;
	int	MODBUS_ANALOG_IN1	;
	int	MODBUS_ANALOG_IN2	;
	int	MODBUS_DIGITAL_IN1	;
	int	MODBUS_OUTPUT1_DELAY_OFF_TO_ON	;
	int	MODBUS_OUTPUT2_DELAY_OFF_TO_ON	;
	int	MODBUS_OUTPUT3_DELAY_OFF_TO_ON	;
	int	MODBUS_OUTPUT4_DELAY_OFF_TO_ON	;
	int	MODBUS_OUTPUT5_DELAY_OFF_TO_ON	;
	int	MODBUS_OUTPUT1_DELAY_ON_TO_OFF	;
	int	MODBUS_OUTPUT2_DELAY_ON_TO_OFF	;
	int	MODBUS_OUTPUT3_DELAY_ON_TO_OFF	;
	int	MODBUS_OUTPUT4_DELAY_ON_TO_OFF	;
	int	MODBUS_OUTPUT5_DELAY_ON_TO_OFF	;
	int	MODBUS_CYCLING_DELAY	;
	int	MODBUS_CHANGOVER_DELAY	;
	int	MODBUS_DISPLAY	;
	int	MODBUS_LED1_CONTROL	;
	int	MODBUS_LED2_CONTROL	;
	int	MODBUS_LED3_CONTROL	;
	int	MODBUS_LED4_CONTROL	;
	int	MODBUS_LED5_CONTROL	;
	int	MODBUS_LED6_CONTROL	;
	int	MODBUS_LED7_CONTROL	;
	int	MODBUS_OVERRIDE_TIMER	;
	int	MODBUS_OVERRIDE_TIMER_LEFT	;
	int	MODBUS_FILTER	;
	int	MODBUS_HEAT_COOL_CONFIG	;
	int	MODBUS_INTERNAL_TEMP_IC	;
	int	MODBUS_INTERNAL_THERMISTOR	;
	int	MODBUS_CALIBRATION_INTERNAL_THERMISTOR	;
	int	MODBUS_CALIBRATION_ANALOG_IN2	;
	int	MODBUS_TABLE1_ZERO	;
	int	MODBUS_TABLE1_HALFONE	;
	int	MODBUS_TABLE1_ONE	;
	int	MODBUS_TABLE1_HALFTWO	;
	int	MODBUS_TABLE1_TWO	;
	int	MODBUS_TABLE1_HALFTHREE	;
	int	MODBUS_TABLE1_THREE	;
	int	MODBUS_TABLE1_HALFFOUR	;
	int	MODBUS_TABLE1_FOUR	;
	int	MODBUS_TABLE1_HALFFIVE	;
	int	MODBUS_TABLE1_FIVE	;
	int	MODBUS_TABLE2_ZERO	;
	int	MODBUS_TABLE2_HALFONE	;
	int	MODBUS_TABLE2_ONE	;
	int	MODBUS_TABLE2_HALFTWO	;
	int	MODBUS_TABLE2_TWO	;
	int	MODBUS_TABLE2_HALFTHREE	;
	int	MODBUS_TABLE2_THREE	;
	int	MODBUS_TABLE2_HALFFOUR	;
	int	MODBUS_TABLE2_FOUR	;
	int	MODBUS_TABLE2_HALFFIVE	;
	int	MODBUS_TABLE2_FIVE	;
	int	MODBUS_INPUT1_SELECT	;
	int	MODBUS_UNIVERSAL_DB_HI	;
	int	MODBUS_UNIVERSAL_DB_LO	;
	int	MODBUS_UNIVERSAL_PTERM	;
	int	MODBUS_UNIVERSAL_ITERM	;
	int	MODBUS_UNIVERSAL_SET	;
	int	MODBUS_PID_OUTPUT1	;
	int	MODBUS_PID_OUTPUT2	;
	int	MODBUS_PID_OUTPUT3	;
	int	MODBUS_PID_OUTPUT4	;
	int	MODBUS_PID_OUTPUT5	;
	int	MODBUS_PID_OUTPUT6	;
	int	MODBUS_PID_OUTPUT7	;
	int	MODBUS_UNIVERSAL_OUTPUT_BEGIN	;
	int	MODBUS_UNIVERSAL_OUTPUT_COOL1	;
	int	MODBUS_UNIVERSAL_OUTPUT_COOL2	;
	int	MODBUS_UNIVERSAL_OUTPUT_COOL3	;
	int	MODBUS_UNIVERSAL_OUTPUT_HEAT1	;
	int	MODBUS_UNIVERSAL_OUTPUT_HEAT2	;
	int	MODBUS_UNIVERSAL_OUTPUT_HEAT3	;
	int	MODBUS_UNIVERSAL_VALVE_BEGIN	;
	int	MODBUS_UNIVERSAL_VALVE_COOL1	;
	int	MODBUS_UNIVERSAL_VALVE_COOL2	;
	int	MODBUS_UNIVERSAL_VALVE_COOL3	;
	int	MODBUS_UNIVERSAL_VALVE_HEAT1	;
	int	MODBUS_UNIVERSAL_VALVE_HEAT2	;
	int	MODBUS_UNIVERSAL_VALVE_HEAT3	;
	int	MODBUS_HEAT_UNIVERSAL_TABLE	;
	int	MODBUS_COOL_UNIVERSAL_TABLE	;
	int	MODBUS_PID_UNIVERSAL	;
	int	MODBUS_UNITS1_HIGH	;
	int	MODBUS_UNITS1_LOW	;
	int	MODBUS_UNITS2_HIGH	;
	int	MODBUS_UNITS2_LOW	;
	int	MODBUS_UNIVERSAL_NIGHTSET	;
	int	MODBUS_HEAT_ORIGINAL_TABLE	;
	int	MODBUS_COOL_ORIGINAL_TABLE	;
	int	MODBUS_PID2_MODE_OPERATION	;
	int	MODBUS_VALVE_TRAVEL_TIME	;
	int	MODBUS_MODE_OUTPUT1	;
	int	MODBUS_MODE_OUTPUT2	;
	int	MODBUS_MODE_OUTPUT3	;
	int	MODBUS_MODE_OUTPUT4	;
	int	MODBUS_MODE_OUTPUT5	;
	int	MODBUS_VALVE_PERCENT	;
	int	MODBUS_INTERLOCK_OUTPUT1	;
	int	MODBUS_INTERLOCK_OUTPUT2	;
	int	MODBUS_INTERLOCK_OUTPUT3	;
	int	MODBUS_INTERLOCK_OUTPUT4	;
	int	MODBUS_INTERLOCK_OUTPUT5	;
	int	MODBUS_INTERLOCK_OUTPUT6	;
	int	MODBUS_INTERLOCK_OUTPUT7	;
	int	MODBUS_SETPOINT_INCREASE	;
	int	MODBUS_LAST_KEY_TIMER	;
	int	MODBUS_FREEZE_TEMP_SETPOIN	;
	int	MODBUS_FREEZE_DELAY_ON	;
	int	MODBUS_FREEZE_DELAY_OFF	;
	int	MODBUS_ANALOG1_FUNCTION	;
	int	MODBUS_ANALOG2_FUNCTION	;
	int	MODBUS_DIGITAL1_FUNCTION	;
	int	MODBUS_TIMER_ON	;
	int	MODBUS_TIMER_OFF	;
	int	MODBUS_TIMER_UNITS	;
	int	MODBUS_KEYPAD_VALUE	;
	int	MODBUS_DISPLAY_HUNDRED	;
	int	MODBUS_DISPLAY_TEN	;
	int	MODBUS_DISPLAY_DIGITAL	;
	int	MODBUS_DISPLAY_STATUS	;
	int	MODBUS_INPUT_MANU_ENABLE	;
	int	MODBUS_OUTPUT_MANU_ENABLE	;
	int	MODBUS_DIGITAL_INPUT	;
	int	MODBUS_MANU_RELAY1	;
	int	MODBUS_MANU_RELAY2	;
	int	MODBUS_MANU_RELAY3	;
	int	MODBUS_MANU_RELAY4	;
	int	MODBUS_MANU_RELAY5	;
	int	MODBUS_DEAD_MASTER	;
	int	MODBUS_ROUND_DISPLAY	;
	int	MODBUS_MIN_ADDRESS	;
	int	MODBUS_MAX_ADDRESS	;
	int	MODBUS_FAN_GRIDPOINT	;
	int	MODBUS_MODE_GRIDPOINT	;
	int	MODBUS_HOLD_GRIDPOINT	;
	int	MODBUS_CONFIGURATION	;
	int	MODBUS_UTIL_MODE	;
	int	MODBUS_EEPROM_SIZE	;
	int	MODBUS_TIMER_SELECT	;
	int	MODBUS_OUTPUT1_FUNCTION	;
	int	MODBUS_ROTATION_OUT2	;
	int	MODBUS_ROTATION_OUT3	;
	int	MODBUS_ROTATION_OUT4	;
	int	MODBUS_ROTATION_OUT5	;
	int	MODBUS_ROTATION_TIME_LEFT	;
	int	MODBUS_OUTPUT2_FUNCTION	;
	int	MODBUS_OUTPUT3_FUNCTION	;
	int	MODBUS_OUTPUT4_FUNCTION	;
	int	MODBUS_OUTPUT5_FUNCTION	;
	int	MODBUS_DEFAULT_SETPOINT	;
	int	MODBUS_SETPOINT_CONTROL	;
	int	MODBUS_PIR_STAGE	;
	int	MODBUS_OUTPUT_PWM_AUTO_COAST	;
	int	MODBUS_OUTPUT_PWM_AUTO_COOL1	;
	int	MODBUS_OUTPUT_PWM_AUTO_COOL2	;
	int	MODBUS_OUTPUT_PWM_AUTO_COOL3	;
	int	MODBUS_OUTPUT_PWM_AUTO_HEAT1	;
	int	MODBUS_OUTPUT_PWM_AUTO_HEAT2	;
	int	MODBUS_OUTPUT_PWM_AUTO_HEAT3	;
	int	MODBUS_PWM_OUT4	;
	int	MODBUS_PWM_OUT5	;
	int	MODBUS_FREE_COOL_CONFIG	;
	int	MODBUS_VALVE_OFF_TABLE_COAST	;
	int	MODBUS_VALVE_OFF_TABLE_COOL1	;
	int	MODBUS_VALVE_OFF_TABLE_COOL2	;
	int	MODBUS_VALVE_OFF_TABLE_COOL3	;
	int	MODBUS_VALVE_OFF_TABLE_HEAT1	;
	int	MODBUS_VALVE_OFF_TABLE_HEAT2	;
	int	MODBUS_VALVE_OFF_TABLE_HEAT3	;
	int	MODBUS_LOCK_REGISTER	;
	int	MODBUS_ANALOG1_RANGE	;
	int	MODBUS_ANALOG2_RANGE	;
	int	MODBUS_ANALOG3_RANGE	;
	int	MODBUS_ANALOG4_RANGE	;
	int	MODBUS_ANALOG5_RANGE	;
	int	MODBUS_ANALOG6_RANGE	;
	int	MODBUS_ANALOG7_RANGE	;
	int	MODBUS_ANALOG8_RANGE	;
	int	MODBUS_ANALOG_INPUT1	;
	int	MODBUS_ANALOG_INPUT2	;
	int	MODBUS_ANALOG_INPUT3	;
	int	MODBUS_ANALOG_INPUT4	;
	int	MODBUS_ANALOG_INPUT5	;
	int	MODBUS_ANALOG_INPUT6	;
	int	MODBUS_ANALOG_INPUT7	;
	int	MODBUS_ANALOG_INPUT8	;
	int	MODBUS_CALIBRATION_ANALOG1	;
	int	MODBUS_CALIBRATION_ANALOG2	;
	int	MODBUS_CALIBRATION_ANALOG3	;
	int	MODBUS_CALIBRATION_ANALOG4	;
	int	MODBUS_CALIBRATION_ANALOG5	;
	int	MODBUS_CALIBRATION_ANALOG6	;
	int	MODBUS_CALIBRATION_ANALOG7	;
	int	MODBUS_CALIBRATION_ANALOG8	;
	int	MODBUS_LINE1_CHAR1	;
	int	MODBUS_LINE1_CHAR2	;
	int	MODBUS_LINE1_CHAR3	;
	int	MODBUS_LINE1_CHAR4	;
	int	MODBUS_LINE1_CHAR5	;
	int	MODBUS_LINE1_CHAR6	;
	int	MODBUS_LINE1_CHAR7	;
	int	MODBUS_LINE1_CHAR8	;
	int	MODBUS_LINE2_CHAR1	;
	int	MODBUS_LINE2_CHAR2	;
	int	MODBUS_LINE2_CHAR3	;
	int	MODBUS_LINE2_CHAR4	;
	int	MODBUS_LINE2_CHAR5	;
	int	MODBUS_LINE2_CHAR6	;
	int	MODBUS_LINE2_CHAR7	;
	int	MODBUS_LINE2_CHAR8	;
	int	MODBUS_LCD_TURN_OFF	;
	int	MODBUS_LCD_SCREEN1	;
	int	MODBUS_LCD_SCREEN2	;
	int	MODBUS_SUN_ICON_CONTROL	;
	int	MODBUS_MOON_ICON_CONTROL	;
	int	MODBUS_TURN_OFF_FAN	;
	int	MODBUS_TEST2	;
	int	MODBUS_TEST3	;
	int	MODBUS_PRESSURE_DATA_WC	;
	int	MODBUS_PRESSURE_DATA_PA	;
	int	MODBUS_FEEDBACK_DATA	;
	int	MODBUS_DAC_DATA	;
	int	MODBUS_PRESSURE_SENSOR_SELECT	;
	int	MODBUS_PRESSURE_UNIT_SELECT	;
	int	MODBUS_PRESSURE_MIN	;
	int	MODBUS_PRESSURE_MAX	;
	int	MODBUS_CAL	;
	int	MODBUS_MODE_SELECT	;
	int	MODBUS_DEFAULT	;
	int	MODBUS_OUTPUT_USER_RANGE	;
	int	MODBUS_PRESSURE_OUTPUT_SELECT	;
	int	MODBUS_PRESSURE_SETPOINT	;
	int	MODBUS_PRESSURE_CAL_POINTS	;
	int	MODBUS_PRESSURE_CAL_ZERO	;
	int	MODBUS_OUTPUT_VOLT_OFFSET	;
	int	MODBUS_OUTPUT_CURRENT_OFFSET	;
	int	MODBUS_OUTPUT_CURRENT_ADJ	;
	int	MODBUS_PRESSURE_CAL_PR1	;
	int	MODBUS_PRESSURE_CAL_VOLT1	;
	int	MODBUS_PRESSURE_CAL_PR2	;
	int	MODBUS_PRESSURE_CAL_VOLT2	;
	int	MODBUS_PRESSURE_CAL_PR3	;
	int	MODBUS_PRESSURE_CAL_VOLT3	;
	int	MODBUS_PRESSURE_CAL_PR4	;
	int	MODBUS_PRESSURE_CAL_VOLT4	;
	int	MODBUS_PRESSURE_CAL_PR5	;
	int	MODBUS_PRESSURE_CAL_VOLT5	;
	int	MODBUS_PRESSURE_CAL_PR6	;
	int	MODBUS_PRESSURE_CAL_VOLT6	;
	int	MODBUS_PRESSURE_CAL_PR7	;
	int	MODBUS_PRESSURE_CAL_VOLT7	;
	int	MODBUS_PRESSURE_CAL_PR8	;
	int	MODBUS_PRESSURE_CAL_VOLT8	;
	int	MODBUS_PRESSURE_CAL_PR9	;
	int	MODBUS_PRESSURE_CAL_VOLT9	;
	int	MODBUS_PRESSURE_CAL_PR10	;
	int	MODBUS_PRESSURE_CAL_VOLT10	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION1	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION2	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION3	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION4	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION5	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION6	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION7	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION8	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION9	;
	int	MODBUS_OUTPUT_VOLT_CALIBRATION10	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION1	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION2	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION3	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION4	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION5	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION6	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION7	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION8	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION9	;
	int	MODBUS_OUTPUT_CURRENT_CALIBRATION10	;
	int	MODBUS_CAL_UNIT	;
	int    MODBUS_PRESSURE_RATIO;
	int	MODBUS_PRESSURE_INDEX;
	int	MODBUS_PRESSURE_ORG_VALUE;
public:
	CComboBox m_combox_unit;
	afx_msg void OnCbnSelchangeBoxunit();
	CComboBox m_cmbox_outputtype;
	afx_msg void OnCbnSelchangeComboOutputtype();
	afx_msg void OnBnClickedButtonOutputtable();
	CComboBox m_keypad_lock;
	CComboBox m_Sensor_Type_Combox;
	CComboBox m_am_combox;
	CComboBox m_combox_output_range;
	CEdit m_input_filter;
	CEdit m_edit_signal;
	CMsflexgrid m_pressure_table;
	double m_output_value;
	double m_signal_value;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeKeypadlockcombo();
	afx_msg void OnCbnSelchangeSensorType();
	afx_msg void OnCbnSelchangeComboAm();
	 /*afx_msg*/ void OnEnKillfocusEditFilter();
	afx_msg void OnCbnSelchangeComboOutputRange();
	 /*afx_msg*/ void OnEnKillfocusPressureSensor();
	/*afx_msg*/  void OnEnKillfocusEditSensorMin();
	/*afx_msg*/  void OnEnKillfocusEditSensorMax();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
   // afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);//�ֶ�����.
	afx_msg void OnEnSetfocusPressureSensor();
	afx_msg void OnEnSetfocusEditFilter();
	afx_msg void OnSetfocusEditSensorMin();
	afx_msg void OnSetfocusEditSensorMax();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

