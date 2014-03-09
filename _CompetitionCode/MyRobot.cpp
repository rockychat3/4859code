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


class ByronMecanum : public IterativeRobot
{
	RobotDrive robotDrive; // robot drive system (1, 6, 8, 7)
	Victor shooter_1;	//shooter 1 V2
	Victor shooter_2;	//shooter 2 V3
	Victor aquire;		//Picker upper 1 V4
	Victor lift;		//Lifter V5 
	
	Joystick stick_1;	// driving
	GamePad xbox;		//all other controls
	
	Gyro gyro; 
	Ultrasonic ultra;
	
	NetworkTable *preferencesTable;
	
	bool ran;
	float voltage;
	

public:
	ByronMecanum():
	robotDrive(1, 6, 8, 7),	// these must be initialized in the same order
	shooter_1(2),
	shooter_2(3),	
	aquire(4),
	lift(5),
	
	stick_1(1),	
	xbox(2),
	
	gyro(1),
	ultra(2, 2) //79 in 
	
	{
		ran = false;
		
		robotDrive.SetExpiration(0.1);
		this->SetPeriod(0); //Set update period to sync with robot control packets (20ms nominal)
		
		robotDrive.SetInvertedMotor(robotDrive.kFrontRightMotor, true);
		robotDrive.SetInvertedMotor(robotDrive.kRearRightMotor, true);
		
		preferencesTable = NetworkTable::GetTable("Preferences");
	}


void ByronMecanum::RobotInit() { 
}


void ByronMecanum::DisabledInit() {
}


void ByronMecanum::DisabledPeriodic() {
}


void ByronMecanum::AutonomousInit() {
	gyro.Reset();
	ran = false;
}


void ByronMecanum::AutonomousPeriodic() {
	if (!ran) {
		for (int i = 0; i<10000; i++) {
			robotDrive.MecanumDrive_Cartesian(0.0, -0.5, 0.0);
		}
		robotDrive.MecanumDrive_Cartesian(0.0, 0.0, 0.0);
		ran = true;
	}
}	


void ByronMecanum::TeleopInit() {
	SmartDashboard::init ();
	
	gyro.Reset();
	ultra.SetAutomaticMode(true);
	preferencesTable->PutNumber("s_1", 0.5);
	preferencesTable->PutNumber("s_2", 0.5);
}
   

void ByronMecanum::TeleopPeriodic() {
	
	double x = stick_1.GetX();          //variable for side to side movement
	double y = stick_1.GetY();          //variable for forward/backward movement
	double z = stick_1.GetZ()/2.3;      //variable for rotation	
	double deadzone = 0.28;	 //variable for amount of deadzone
	x = ((fabs(x) < deadzone) ? (0) : (x));
	y = ((fabs(y) < deadzone) ? (0) : (y));	
	z = ((fabs(z) < deadzone) ? (0) : (z));
	
	//double q = preferencesTable->GetNumber("s_1", 0.0);
	//double p = preferencesTable->GetNumber("s_2", 0.0);

	voltage = DriverStation::GetInstance()->GetBatteryVoltage();
	SmartDashboard::PutNumber("Voltage", voltage);
	
	// "B" to kill shooter
	if (xbox.GetButton02()){
		shooter_1.Set(0.0);
		shooter_2.Set(0.0);
	// "A" to spin up shooter
	} else if(xbox.GetButton01()){
		shooter_1.Set(12.0/voltage);
		shooter_2.Set(12.0/voltage);
	}
	
	// "X" to lift
	if(xbox.GetButton03()) {
		lift.Set(-1.0);
	// "Y" to set lifter down
	} else if (xbox.GetButton04()) {
		lift.Set(0.3);
	} else {
		lift.Set(0.0);	
	}
	
	// "LeftTrigger" to acquire ball
	if (xbox.GetButton05()){
		aquire.Set(1.0);
	// "RightTrigger" to spit out ball
	} else if (xbox.GetButton06()) {
		aquire.Set(-1.0);
	} else {
		aquire.Set(0.0);
	}
	
	SmartDashboard::PutNumber("x",stick_1.GetX());
	SmartDashboard::PutNumber("y",stick_1.GetY());
	SmartDashboard::PutNumber("z",stick_1.GetZ());
	SmartDashboard::PutNumber("Gyro",gyro.GetAngle());
	SmartDashboard::PutNumber("Range", ultra.GetRangeInches());
	
	robotDrive.MecanumDrive_Cartesian(x, y, z);
}


void ByronMecanum::TestInit() {
}


void ByronMecanum::TestPeriodic() {
}

};
START_ROBOT_CLASS(ByronMecanum);
