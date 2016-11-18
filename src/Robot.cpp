#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser,*chooser2;
	const std::string autoNameDefault = "Forward 4 seconds";
	const std::string autoNameCustom = "Backward 4 seconds and lower shooter";
	const std::string autoNameNone = "Do nothing in auton";
	const std::string tankDrive = "Tank Drive";
	const std::string gtaDrive = "GTA Drive (Don't use this yet its broke)";
	std::string autoSelected;
	Joystick *primaryStick,*secondaryStick;
	RobotDrive *primaryDrive;
	CANTalon *frontLeft,*backLeft,*frontRight,*backRight,*shooterPitch,*shooterGrabber;
	bool driveModeGTA = false;
	bool autoForward;
	bool autoBackward;
	bool autoNone;

	void RobotInit()
	{
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		chooser->AddObject(autoNameNone, (void*)&autoNameNone);
		SmartDashboard::PutData("Auto Modes", chooser);
		chooser2 = new SendableChooser();
		chooser2->AddDefault(tankDrive,(void*)&tankDrive);
		chooser2->AddObject(gtaDrive,(void*)&gtaDrive);
		SmartDashboard::PutData("Control Modes",chooser2);
		frontLeft = new CANTalon(2);
		backLeft = new CANTalon(4);
		frontRight = new CANTalon(6);
		backRight = new CANTalon(8);
		shooterPitch = new CANTalon(10);
		shooterGrabber = new CANTalon(12);
		primaryStick = new Joystick(0);
		secondaryStick = new Joystick(1);

		autoNone = true;
		autoForward = false;
		autoBackward = false;

		primaryDrive = new RobotDrive(frontLeft,backLeft,frontRight,backRight);
		primaryDrive->SetSafetyEnabled(false);
		shooterGrabber->SetSafetyEnabled(false);
	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		//std::cout << "Auto selected: " << autoSelected << std::endl;
		/*/
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
			shooterPitch->Set(-1);
			Wait(1);
			shooterPitch->Set(0);
			primaryDrive->Drive(1,0);
			Wait(4);
			primaryDrive->Drive(0,0);
		} else if (autoSelected == autoNameDefault) {
			//Default Auto goes here
			primaryDrive->Drive(-1,0);
			Wait(4);
			primaryDrive->Drive(0,0);
		}
		else{

		}
		/*/

		if (autoNone)
		{

		}else if (autoForward)
		{
			primaryDrive->Drive(-0.75,0);
			Wait(1.5);
			primaryDrive->Drive(0,0);
		}else if (autoBackward)
		{
			shooterPitch->Set(-1);
			Wait(2.2);
			shooterPitch->Set(0);
			primaryDrive->Drive(0.75,0);
			Wait(1.5);
			primaryDrive->Drive(0,0);
		}


	}

	void AutonomousPeriodic()
	{
		/*/float curve = SmartDashboard::GetNumber("targetX",0);
		if (curve!=0 and curve<=1)
		{
			primaryDrive->ArcadeDrive(0,0.5*curve,false);
		}
		else
		{
			primaryDrive->ArcadeDrive(0,0,false);
		}/*/
	}

	void TeleopInit()
	{
		/*/if (*((std::string*)chooser2->GetSelected()) == tankDrive)
		{
			driveModeGTA = false;
		}
		else
		{
			driveModeGTA = true;
		}
		/*/
	}

	void TeleopPeriodic()
	{
		if(driveModeGTA)
		{
			if (primaryStick->GetRawAxis(2)>0.05) // break / reverse
			{
				primaryDrive->Drive(primaryStick->GetRawAxis(2),primaryStick->GetRawAxis(0));
			} else if (primaryStick->GetRawAxis(3)>0.05) // forwards
			{
				primaryDrive->Drive(-primaryStick->GetRawAxis(3),primaryStick->GetRawAxis(0));
			} else
			{
				primaryDrive->Drive(0,primaryStick->GetRawAxis(0));
			}
		}
		else
		{
			primaryDrive->TankDrive(primaryStick->GetRawAxis(1),primaryStick->GetRawAxis(5)); // these might need to go negative
		}

		shooterPitch->Set(secondaryStick->GetY());
		if (secondaryStick->GetRawButton(1))
		{
			shooterGrabber->Set(1);
		}else if (secondaryStick->GetRawButton(3))
		{
			shooterGrabber->Set(-1);
		}else
		{
			shooterGrabber->Set(0);
		}


		/*/ XBOX-Style Driving code:

		/*/

		// todo: make a sweet xbox controller object to make adding new joystick control super easy
		// will i ever actually do it? only time will tell
		// another todo:
		// customizable loadouts on a per-driver basis. Also suggest a practicing schedule where the
		// truly best drivers are selected..... for real. Some sort of course where so many points
		// can be scored. Maybe based on time and a few accuracy points or something. but really needs
		// to be done.
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	void DisabledPeriodic()
	{
		if(primaryStick->GetRawButton(1))
		{
			autoBackward = true;
			autoForward = false;
			autoNone = false;
			std::cout << "Auto mode set to backward and lower shooter\n";
		}else if(primaryStick->GetRawButton(4))
		{
			autoBackward = false;
			autoForward = true;
			autoNone = false;
			std::cout << "Auto mode set to forward\n";
		}else if(primaryStick->GetRawButton(2) or primaryStick->GetRawButton(3))
		{
			autoBackward = false;
			autoForward = false;
			autoNone = true;
			std::cout << "Auto mode set to do nothing\n";
		}

		if(primaryStick->GetRawButton(7))
		{
			driveModeGTA = false;
			std::cout << "Drive mode set to tank-style\n";
		} else if (primaryStick->GetRawButton(8))
		{
			driveModeGTA = true;
			std::cout << "Drive mode set to GTA-style\n";
		}


	}

	void DisabledInit()
	{
	}

};

START_ROBOT_CLASS(Robot)
