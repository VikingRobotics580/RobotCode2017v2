#include <WPILib.h>
#include <stdio.h>

#include <logging.h>

/*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
 * The Robot Load Order, after Calling START_ROBOT_CLASS, is as follows:       *
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
            joy2 = new Joystick(1); //Shooter Joystick
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
            if (clock.Get() <= 2.2){
                drive->MecanumDrive_Cartesian(0.0f, -0.5f, 0.0f, 0.0f); //-y Axis is FORWARD

            } else {
            	clock.Stop();
                drive->MecanumDrive_Cartesian(0.0f, 0.0f, 0.0f, 0.0f);

            };

        }

        void TeleopInit() {
            log_info("Initializing Teleop mode.\n");
            clock.Reset();
        }

        void TeleopPeriodic() {

            //Drive Train
        	// During teleop we want to pass Mecanum Drive the joystick axes.
            // void MecanumDrive_Cartesian(float x, float y, float rotation, float gyro)
            drive->MecanumDrive_Cartesian(joy->GetX(), joy->GetY(),
                                          joy->GetTwist(), 0.0f);

            //Gears 1 and 2
            if(joy2->GetRawButton(10)) { // button 10 on the joystick will initiate the gear stopper
            	gear1.Set(1);
            	gear2.Set(0);
            } else {
            	gear1.Set(0.45);
            	gear2.Set(0.55);
            }

            //Trigger
            if(joy2 ->GetRawButton(1)){ // trigger on the joystick/adruino will release the balls into the motor.
            	ballReleaser.Set(0);
            } else{
            	ballReleaser.Set(0.5);
            }
            //Shooter
            if(joy2->GetRawButton(2)) { // starts up the motor when button 12 is pressed
            	shooter.Set(((((joy2->GetThrottle())*-1)+1)/4)+0.25); //This puts power between 50% and 100%
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

