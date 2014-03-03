#include "WPILib.h"
#include "Math.h"
#include "Victor.h"
#include "Ultrasonic.h"
#include "GamePad.h"
#include "DriverStation.h"
#include "Preferences.h"
#include "networktables\NetworkTable.h"
#include "networktables\NetworkTableConnectionListenerAdapter.h"
#include "networktables\NetworkTableKeyListenerAdapter.h"
#include "networktables\NetworkTableListenerAdapter.h"
#include "networktables\NetworkTableMode.h"
#include "networktables\NetworkTableProvider.h"
#include "networktables\NetworkTableSubListenerAdapter.h"
#include "SmartDashboard\SmartDashboard.h"
#include "SmartDashboard\NamedSendable.h"
#include "SmartDashboard\Sendable.h"
#include "SmartDashboard\SendableChooser.h"
#include "tables\IRemote.h"
#include "tables\IRemoteConnectionListener.h"
#include "tables\ITable.h"
#include "tables\ITableListener.h"
#include "tables\ITableProvider.h"
#include "tables\TableKeyNotDefinedException.h"
#include "tables\IRemoteConnectionListener.h"
#include "NetworkCommunication\AICalibration.h"
#include "NetworkCommunication\FRCComm.h"
#include "NetworkCommunication\LoadOut.h"
#include "NetworkCommunication\symModuleLink.h"
#include "NetworkCommunication\UsageReporting.h"
#include "LiveWindow\LiveWindow.h"
#include "LiveWindow\LiveWindowSendable.h"
#include "LiveWindow\LiveWindowStatusListener.h"

//#include "Compressor.h"
//#include "Solenoid.h"

	
/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 


class ByronMecanum : public IterativeRobot
{
	RobotDrive robotDrive; // robot drive system (1, 6, 8, 7)
	Victor shooter_1;  //shooter 1 V2
	Victor shooter_2;  //shooter 2 V3
	Victor aquire; //Lifter 1 V4
	Victor cradle_lift;	//Picker upper V5 
	Victor down;  //Make it out V9
	
	Joystick stick_1; // driving
	//Joystick stick_2; // shooting
	GamePad xbox;	//all other controls

	Gyro gyro; 
	Ultrasonic ultra;
	
	NetworkTable *preferencesTable;
	
	bool ran;
	
	float voltage;
	float GetBatteryVoltage;

	
	
//	Compressor *m_compressor; 
//	bool solenoid_1;
//	bool solenoid_2;
//	bool solenoid_3;
//	bool solenoid_4;
	

public:
	ByronMecanum():
		robotDrive(1, 6, 8, 7),	// these must be initialized in the same order
		shooter_1(2),
		shooter_2(3),	
		aquire(4),
		cradle_lift(5),
		down(9),
		
		stick_1(1),		// as they are declared above.	
		//stick_2(2),		// same 
		xbox(2),
		
		gyro(1),
	    ultra(2, 2) 
	{
		
		ran = false;
		
		robotDrive.SetExpiration(0.1);
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
		
		robotDrive.SetInvertedMotor(robotDrive.kFrontRightMotor, true);
		robotDrive.SetInvertedMotor(robotDrive.kRearRightMotor, true);
		
		preferencesTable = NetworkTable::GetTable("Preferences");

	

		//Compressor *c = new Compressor(4, 2);  c->Start(); 
//		solenoid_1 = false;
//		solenoid_2 = false;
//		solenoid_3 = false;
//		solenoid_4 = false;
				

	}
	
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 * Image processing code to identify 2013 Vision targets
 */
	
void ByronMecanum::RobotInit() {
	//Compressor *c = new Compressor(4, 2);  c->Start(); 
	ran = false;
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void ByronMecanum::DisabledInit() {
	gyro.Reset();
	//delete m_compressor;
	ran = false;

}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void ByronMecanum::DisabledPeriodic() {
	//delete m_compressor;


}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void ByronMecanum::AutonomousInit() 

{
	gyro.Reset();
	ran = false;
		
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void ByronMecanum::AutonomousPeriodic() {
	
	if (!ran) {
		for (int i = 0; i<10000; i++) {
			robotDrive.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		}
		//Wait(10.0);
		robotDrive.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
		ran = true;
	}

}	
/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void ByronMecanum::TeleopInit() {
	SmartDashboard::init ();
	
	gyro.Reset();
	ultra.SetAutomaticMode(true);

}
    

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void ByronMecanum::TeleopPeriodic() {

	double x = stick_1.GetX();          //variable for side to side movement
	double y = stick_1.GetY();          //variable for forward/backward movement
	double z = stick_1.GetZ()/2.3;      //variable for rotation	
	double deadzone = 0.28;				//variable for amount of deadzone
/*if (sqrt(pow(x, 2.0) + pow(y, 2.0)) < deadzone){
		gyro.Reset();
	}*/
	x = ((fabs(x) < deadzone) ? (0) : (x));
	y = ((fabs(y) < deadzone) ? (0) : (y));	
	z = ((fabs(z) < deadzone) ? (0) : (z));
	
	//shooter_1.Set(1.0);
	//shooter_2.Set(1.0);
	
	if (xbox.GetButton01()){
		shooter_1.Set(0.0);
		shooter_2.Set(0.0);				
		cradle_lift.Set(-1.0);
		Wait(0.3);
		cradle_lift.Set(0.0);	
	}	
	if (xbox.GetButton02()) {
		cradle_lift.Set(1.0);
		Wait(0.3);
		cradle_lift.Set(0.0);
	}
	if(xbox.GetButton03()){
		shooter_1.Set(11.8/voltage);
		shooter_2.Set(11.8/voltage);
	}
	if(xbox.GetRockerX()) {
			down.Set(1.0);
			Wait(0.5);
			down.Set(0.0);	
		}
	if(xbox.GetLeftY()){
		aquire.Set(xbox.GetLeftY());
	} 
	
	if(xbox.GetButton05()){
		shooter_1.Get();
		shooter_2.Set();
	}
		
	
	preferencesTable->PutString("s_1", "998899");
	preferencesTable->GetString("s_1", "Empty");
	
	preferencesTable->PutString("s_2", "0.0");
	preferencesTable->GetString("s_2", "Empty");
	
		
	
	voltage = DriverStation::GetInstance()->GetBatteryVoltage();
	SmartDashboard::PutNumber("Voltage", voltage);
	
	SmartDashboard::PutNumber("x",stick_1.GetX());
	SmartDashboard::PutNumber("y",stick_1.GetY());
	SmartDashboard::PutNumber("z",stick_1.GetZ());
	SmartDashboard::PutNumber("Gyro",gyro.GetAngle());
	SmartDashboard::PutNumber("Range", ultra.GetRangeInches());
	//SmartDashboard::PutNumber("z2",stick_2.GetZ());
	//SmartDashboard::PutNumber("throt2",stick_2.GetThrottle());
	//SmartDashboard::PutNumber("Twist", stick_2.GetTwist());

	//System.out.println( "pref1 = " + preferencesTable->GetString("pref1", "Empty"));
		
	/*if(xbox.GetLeftY()){
		aquire.Set(xbox.GetLeftY());
	} 	
	
	if(xbox.GetButton04()) {
		down.Set(1.0);
		Wait(0.5);
		down.Set(0.0);
	}
	
	if(xbox.GetButton()) {
		shooter_1.Set(1.0);
		shooter_2.Set(1.0);
	} else {
		shooter_1.Set(0.0);
		shooter_2.Set(0.0);
	}
	
	if(xbox.GetRockerY()) {
		cradle_lift.Set(1.0);
		Wait(0.3);
		cradle_lift.Set(-1.0);
		Wait(0.3);
		cradle_lift.Set(0.0);
	} */
	
	
	/*		
		if(stick_2.GetTrigger()){
			shooter_1.Set(stick_2.GetY()*-1.0);
			shooter_2.Set(stick_2.GetY()*-1.0);
		}	else {
			shooter_1.Set(0.0);
			shooter_2.Set(0.0);
		}
		 
		if (stick_2.GetRawButton(3)) {
			cradle_lift.Set(stick_2.GetThrottle());
		}
		else {
			cradle_lift.Set(0.0);
		}

		if (stick_2.GetRawButton(2)) {
			aquire.Set(stick_2.GetThrottle());
		}	else {
			aquire.Set(0.0);
		}
		
		if (stick_2.GetRawButton(4)) {
			down.Set(1.0);
		}	else {
			down.Set(0.0);
		}
		if (stick_2.GetRawButton(3)) {
			cradle_lift.Set(stick_2.GetThrottle());
		}
		else {
			cradle_lift.Set(0.0);
		}

		if (stick_2.GetRawButton(2)) {
			aquire.Set(stick_2.GetThrottle());
		}	else {
			aquire.Set(0.0);
		}
		
		if (stick_2.GetRawButton(4)) {
			down.Set(1.0);
		}	else {
			down.Set(0.0);
		} 
		*/
			

		
	//	solenoid_1 = xbox.GetButton05();
	//	solenoid_2 = xbox.GetButton06();
	//	solenoid_3 = xbox.GetRockerX();
	//	solenoid_4 = xbox.GetRockerY();
		
	
	robotDrive.MecanumDrive_Cartesian(x, y, z);
	}
/**
 * Initialization code for test mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void ByronMecanum::TestInit() {
}

/**
 * Periodic code for test mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void ByronMecanum::TestPeriodic() {
}

};

START_ROBOT_CLASS(ByronMecanum);

