#include <WPILib.h>
#include <stdio.h>

#include <logging.h>

/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * The Robot Load Order, after calling START_ROBOT_CLASS, is as follows:       *
 *                                                                             *
 * Constructor -> RobotInit -> DisabledInit -> DisabledPeriodic                *
 * Once a mode changes, the function execution order looks like the following: *
 * PreviousModePeriodic -> NextModeInit -> NextModePeriodic                    *
 *                                                                             *
 *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  */

class Robot : public IterativeRobot {
    public:
	Robot():
            IterativeRobot(), //constructor
            //These constructions are given the number of the PWM port on the roboRio
			frontLeft(0),  	//port 0
			backLeft(1),   	//port 1
			frontRight(2), 	//port 2
			backRight(3),	//port 3
			shooter(4),		//port 4
			gear1(5),		//port 5
			gear2(6),		//port 6
    		ballReleaser(7),//port 7
    		agitator(8), 	//port 8
            clock()         //No port
        {
            // You should only initialize value here. Try not to have any other
            //  executable code here.
            joy = new Joystick(0);  //Drive Joystick
            joy2 = new Joystick(1); //Arduino "Joystick"
            drive = NULL;

        }

        void RobotInit() {
            // Initialize the Robot and all functions that need to be called.
            printf("Initializing Robot.\n");
            drive = new RobotDrive(frontLeft, backLeft, frontRight, backRight);
            drive->SetSafetyEnabled(false); // This is dangerous, but our robot
                                            //  has some problems and needs
                                            //  this.
        }

        void AutonomousInit() {
            // Here is where you would initialize all of your autonomous code
            log_info("Initializing Autonomous mode.");
            clock.Start();

        }

        void AutonomousPeriodic() {
            // Here is where you would put all of your autonomous code.
            // It is important to note that all code here will run as though
            //  autonomous mode is endless, so you have to carefully time how
            //  long your code will run for.

        	if (clock.Get() <= 5){
        		drive->MecanumDrive_Cartesian(0.25f, 0.0f, 0.0f, 0.0f);
        	} else {
        		drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);
        	}

        	//Robot Starting Position Scenarios
            //Scenario 1 - blue side, left gear
            if(joy2->GetRawButton(0)) {

            }

            //Scenario 2 - blue side, middle gear
            if(joy2->GetRawButton(1)) {

            }

            //Scenario 3 - blue side, right gear
            if(joy2->GetRawButton(2)) {

            }

            //Scenario 4 - blue side, left shoot
            if(joy2->GetRawButton(3)) {

            }

            //Scenario 5 - blue side, middle shoot
            if(joy2->GetRawButton(4)) {

            }

            //Scenario 6 - blue side, right shoot
            if(joy2->GetRawButton(5)) {

            }

            //Scenario 7 - red side, left gear
            if(joy2->GetRawButton(6)) {

            }

            //Scenario 8 - red side, middle gear
            if(joy2->GetRawButton(7)) {

            }

            //Scenario 9 - red side, right gear
            if(joy2->GetRawButton(8)) {

            }

            //Scenario 10 - red side, left shoot
            if(joy2->GetRawButton(9)) {

            }

            //Scenario 11 - red side, middle shoot
            if(joy2->GetRawButton(10)) {

            }

            //Scenario 12 - red side, right shoot
            if(joy2->GetRawButton(11)) {

            }

        }

        void TeleopInit() {
            log_info("Initializing Teleop mode.\n");
            // We don't have anything to initialize here.
        }

        void TeleopPeriodic() {

        	// During teleop we want to pass Mecanum Drive the joystick axes.
            // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
            // Note that these axes may be reversed, so you may need to switch
            //  the GetX and GetY functions
            drive->MecanumDrive_Cartesian(joy->GetX(), joy->GetY(),
                                          joy->GetTwist(), 0.0f);


            //Gears 1 and 2
            if(joy2->GetRawButton(10)) { // button 11 on the joystick will initiate the gear stopper
            	gear1.Set(1);
            	gear2.Set(0);
            } else {
            	gear1.Set(0.25);
            	gear2.Set(0.75);
            }

            //Trigger
            if(joy2 ->GetRawButton(1)){ // trigger on the joystick/adruino will release the balls into the motor.
            	ballReleaser.Set(0.5);
            } else {
            	ballReleaser.Set(0);
            }
            //Shooter
            if(joy2->GetRawButton(2)) { // starts up the motor when button 12 is pressed
            	shooter.Set(((joy->GetThrottle()+1)/4)+0.25); //This puts power between 50% and 100%
            	//IMPORTANT NOTE: THIS HAS BEEN SET UP FOR ARDUINO
        	} else {
            	shooter.Set(0.0f); //Aka: do nothing
        	}

        }

        void DisabledInit() {
            // Anything you need to initialize in disabled mode
            log_info("Start of Disabled mode.\n");
        }

        void DisabledPeriodic() {
            // This method runs periodically during disabled mode. However, you
            //  cannot do any sort of movement code in here legally, so it is
            //  only here for some basic error and logging utility.
            // Also, during this mode the motors are completely disabled during
            //  this mode anyway, so don't bother
        }

        // You can also have a Test mode, though this will only run if you
        //  specifically start it from your driver station. It will not be
        //  run during the competitions.

    private:

        Joystick* joy;       //Drive Joystick
        Joystick* joy2;      //Arduino "Joystick"
        Talon frontLeft;     //Front-left Mecanum wheel
        Talon backLeft;      //Back-left Mecanum wheel
        Talon frontRight;    //Front-right Mecanum wheel
        Talon backRight;     //Back-right Mecanum wheel
        Talon shooter;       //Shooter Motor
        RobotDrive* drive;   //Drive Train
        Servo gear1;         //Gear 1
        Servo gear2;         //Gear 2
        Servo ballReleaser;           //hopper servo that releases ball when pressed.
        Talon agitator;		 //
        Timer clock;
};

START_ROBOT_CLASS(Robot);

