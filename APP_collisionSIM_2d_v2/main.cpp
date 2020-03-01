
#include <iostream>
#include <math.h>
#include <cmath>
#define PI 3.14159265       // set PI as constant

using namespace std;
static double empty[] = {0,0};      // empty vector used for distances later

// trigonmetric functions for degree inputs instead of rad
double cosDeg(double theta){
    return cos(theta * PI / 180);
}
double sinDeg(double theta){
    return sin(theta * PI / 180);
}

// return distance between objects's origins using Pythagorean theorem
double originDist(double a_s[], double b_s[], double a_rad, double b_rad){
    return pow(pow(a_s[0]-b_s[0], 2) + pow(a_s[1]-b_s[1], 2), 0.5);
}

// check if objects are colliding (NOT including just touching)
bool isColliding(double a_s[], double b_s[], double a_rad, double b_rad){
    double dis = originDist(a_s, b_s, a_rad, b_rad);
    
    // objects are in collision if the sum of their radii exceeds their origins' distance
    if (dis - a_rad - b_rad < 0)
        return true;
    return false;
}

// return theta degree from A to B
double returnTheta(double a_s[], double b_s[]){
    
    // return 90 or 270 if x values are equal
    if (a_s[0] == b_s[0]){
        if (a_s[1] < b_s[1]){
            return 90;
        }
        else{
            return 270;
        }
    }
    // return 0 or 180 if y values are equal
    else if (a_s[1] == b_s[1]){
        if (a_s[0] < b_s[0]){
            return 0;
        }
        else{
            return 180;
        }
    }
    else if (a_s[0]-b_s[0] < 0 && a_s[1]-b_s[1] < 0){       // Quadrant 1
        return atan(abs(a_s[1]-b_s[1])/abs(a_s[0]-b_s[0])) * 180 / PI;
    }
    else if (a_s[0]-b_s[0] > 0 && a_s[1]-b_s[1] < 0){       // Quadrant 2
        return 180 - atan(abs(a_s[1]-b_s[1])/abs(a_s[0]-b_s[0])) * 180 / PI;
    }
    else if (a_s[0]-b_s[0] > 0 && a_s[1]-b_s[1] > 0){       // Quadrant 3
        return 180 + atan(abs(a_s[1]-b_s[1])/abs(a_s[0]-b_s[0])) * 180 / PI;
    }
    else if (a_s[0]-b_s[0] < 0 && a_s[1]-b_s[1] > 0){       // Quadrant 4
        return 360 - atan(abs(a_s[1]-b_s[1])/abs(a_s[0]-b_s[0])) * 180 / PI;
    }
    cout << "ANGLE ERROR";
    return 0;
}

// update the terminal with the current positions and velocities of the objects in the desired form
// also indicate whether the balls are in collision (overlapped) on the current frame/moment
void printStats(double t_ran, double A_pos[], double A_vel[], double B_pos[], double B_vel[], string velPrinting, bool isColliding){
   
    // if velocities are desired to be shown in rectangular, convert each and show the x y components
    if (velPrinting != "p"){
        cout << "\nt: ";
        printf("%.8f", t_ran);
        printf(" |A_s: %.4f, %.4f", A_pos[0], A_pos[1]);
        printf(" |A_v: %.4f, %.4f", A_vel[0]*cosDeg(A_vel[1]),  A_vel[0]*sinDeg(A_vel[1]));
        printf(" |B_s: %.4f, %.4f", B_pos[0], B_pos[1]);
        printf(" |B_v: %.4f, %.4f", B_vel[0]*cosDeg(B_vel[1]),  B_vel[0]*sinDeg(B_vel[1]));
    }
    // if velocities are desired to be shown in polar, show straight from variables
    else{
        cout << "\nt: ";
        printf("%.8f", t_ran);
        printf(" |A_s: %.4f, %.4f", A_pos[0], A_pos[1]);
        printf(" |A_v: %.4f, %.4f", A_vel[0], A_vel[1]);
        printf(" |B_s: %.4f, %.4f", B_pos[0], B_pos[1]);
        printf(" |B_v: %.4f, %.4f", B_vel[0], B_vel[1]);
    }
    // indicate that collision is happening at the current frame
    if (isColliding == true){
        printf(" COLL");
    }
}

// MAIN
int main(int argc, const char * argv[]) {
    
    double t_ran = 0;                   // seconds elapsed
    double t_inc = 0.0000001;           // t_step in seconds
    double updatesPerSec = 100;         // default number of times to print per second
    
    double A_kVal = 1;                  // default k values for both objects
    double B_kVal = 1;
    
    double A_mass = 0.0000001;          // masses in kg
    double B_mass = 0.0000001;
    
    double A_rad = 0.0001;              // radii for each object
    double B_rad = 0.0001;
    
    double A_pos[] = {1,-0.057};        // position vectors for each particle
    double A_vel[] = {0,0};             // and velocity, in polar form (magnitude, angle)
    
    double B_pos[] = {1.750,0.495};
    double B_vel[] = {0,0};
    
    double A_vel_comps[] = {0,0};       // used for storing temporary velocities in rectangular
    double B_vel_comps[] = {0,0};
 
    string choice;                      // variables used to store layout and entry choices
    string velInput;
    string velPrinting;
    string timeStop;
    string massRadiusChoice;
    
    // user input for layout and entry choices
    cout << "Enter p to treat as particles, or anything else to set dimensions: ";
    cin >> choice;
    cout << "Enter p to input velocities in polar form, or anything else for rectangular: ";
    cin >> velInput;
    cout << "Enter p to show velocities over time in polar form, or anything else for rectangular: ";
    cin >> velPrinting;
    cout << "Enter s to auto-stop sim after collision, t to only see collision frames, or anything else to enter in desired duration: ";
    cin >> timeStop;
    cout << "Enter m or r to need to input only mass or only radius, or anything else to input all data: ";
    cin >> massRadiusChoice;

    // if the user wants to set custom dimensions (not particle)
    if (choice != "p"){
        // if user enters mass, the radius can be found by square rooting the mass
        // since mass is proportional to radius squared
        // auto-set the k value as 1
        if (massRadiusChoice == "m"){
            cout << "\nObject A's mass: "; cin >> A_mass;
            cout << "Object B's mass: "; cin >> B_mass;
            A_rad = pow(A_mass, 0.5);
            B_rad = pow(B_mass, 0.5);
            A_kVal = 1;
            B_kVal = 1;
        }
        
        // if user enters radius, the mass can be found my squaring the mass
        // since mass is proportional to radius squared
        // auto-set the k value as 1
        else if (massRadiusChoice == "r"){
            cout << "\nObject A's radius: "; cin >> A_rad;
            cout << "Object B's radius: "; cin >> B_rad;
            A_mass = pow(A_rad, 2);
            B_mass = pow(B_rad, 2);
            A_kVal = 1;
            B_kVal = 1;
        }
        
        // allow user to also enter all data if desired
        else {
            cout << "\nObject A's mass: "; cin >> A_mass;
            cout << "Object B's mass: "; cin >> B_mass;
            cout << "\nObject A's radius: "; cin >> A_rad;
            cout << "Object B's radius: "; cin >> B_rad;
            cout << "\nObject A's k-value: "; cin >> A_kVal;
            cout << "Object B's k-value: "; cin >> B_kVal;
        }
    }
    
    // user inputs each object's initial positions
    cout << "\nObject A's x-pos: ";cin >> A_pos[0];
    cout << "Object A's y-pos: "; cin >> A_pos[1];
    cout << "\nObject B's x-pos: "; cin >> B_pos[0];
    cout << "Object B's y-pos: "; cin >> B_pos[1];
    
    if (velInput != "p"){           // user entered in cartesian; convert to polar and store

        cout << "\nObject A's x-vel: "; cin >> A_vel_comps[0];
        cout << "Object A's y-vel: "; cin >> A_vel_comps[1];
        cout << "\nObject B's x-vel: "; cin >> B_vel_comps[0];
        cout << "Object B's y-vel: "; cin >> B_vel_comps[1];
        
        // find magnitude and angle of velocity for both objects
        
        A_vel[0] = pow(pow(A_vel_comps[0],2) + pow(A_vel_comps[1],2) ,0.5);
        A_vel[1] = returnTheta(empty, A_vel_comps);
        B_vel[0] = pow(pow(B_vel_comps[0],2) + pow(B_vel_comps[1],2) ,0.5);
        B_vel[1] = returnTheta(empty, B_vel_comps);
    }
    else { // user already entered in polar; store directly
        cout << "\nObject A's vel magnitude: "; cin >> A_vel[0];
        cout << "Object A's vel theta: "; cin >> A_vel[1];
        cout << "\nObject B's vel magnitude: "; cin >> B_vel[0];
        cout << "Object B's vel theta: "; cin >> B_vel[1];
    }
    
    // set duration default high
    double duration = 9999;
    // choice of when to stop sim
    if (timeStop != "s" && timeStop != "t"){
        cout << "\nDuration: ";
        cin >> duration;
    }
    if (timeStop != "t"){
        cout << "\nPrints per second (recommended 10 or 100): ";
        cin >> updatesPerSec;
    }
    double radSum = A_rad + B_rad;  // sum of radii used in calculations
    
    // we need to establish a check for when the balls have ever collided, then after that check is true
    // stop the simulation two seconds after the collision ends
    bool collisionDidHappen = false;
    double postCollisionTime = 2;
    
    // only print stats if not only printing for collision frames
    if (timeStop != "t"){
        printStats(t_ran, A_pos, A_vel, B_pos, B_vel, velPrinting, isColliding(A_pos, B_pos, A_rad, B_rad));     // print initial positions/vel
    }
    
    // SIMULATION START
    while (t_ran < duration){
        
        t_ran += t_inc;      // time increment
        
        // check after each increment whether the balls are superimposed at any points
        if (isColliding(A_pos, B_pos, A_rad, B_rad) == true){
            
            collisionDidHappen = true;
            
            // find the angle to the 'other' object for both objects
            double A_contactAngle = returnTheta(A_pos, B_pos);
            double B_contactAngle = fmod(A_contactAngle + 180,360);
            
            // find compression distance using difference in origin distance and radii sum
            // completes the direction and magnitude of compression
            double compressionDist = radSum - originDist(A_pos, B_pos, A_rad, B_rad);
            
            // find the accleration for each object using the compression
            double A_accel[] = {compressionDist*A_kVal/A_mass, B_contactAngle};
            double B_accel[] = {compressionDist*B_kVal/B_mass, A_contactAngle};
            
            // new velocity of each component is the old component + new component from accel
            double A_vel_x = A_vel[0]*cosDeg(A_vel[1]) + A_accel[0]*cosDeg(A_accel[1]);
            double A_vel_y = A_vel[0]*sinDeg(A_vel[1]) + A_accel[0]*sinDeg(A_accel[1]);
            double B_vel_x = B_vel[0]*cosDeg(B_vel[1]) + B_accel[0]*cosDeg(B_accel[1]);
            double B_vel_y = B_vel[0]*sinDeg(B_vel[1]) + B_accel[0]*sinDeg(B_accel[1]);
            
            // create velocity component vectors to be converted back to polar
            double A_vel_comps[] = {A_vel_x, A_vel_y};
            double B_vel_comps[] = {B_vel_x, B_vel_y};
            
            // store new velocities in polar
            A_vel[0] = pow( pow(A_vel_x, 2) + pow(A_vel_y, 2), 0.5);        // velocity magnitude
            A_vel[1] = returnTheta(empty, A_vel_comps);                     // velocity direction/angle
            B_vel[0] = pow( pow(B_vel_x, 2) + pow(B_vel_y, 2), 0.5);
            B_vel[1] = returnTheta(empty, B_vel_comps);

        } // isColliding == true
        else {
            // we only want to decrement the 2-second post-collision timer if the collision already happened
            if (collisionDidHappen == true){
                if (postCollisionTime <= 0){
                    return 0;
                }
                postCollisionTime -= t_inc;
            }
        }
        
        // add current velocity components to the position
        A_pos[0] += A_vel[0]*cosDeg(A_vel[1])*t_inc;
        A_pos[1] += A_vel[0]*sinDeg(A_vel[1])*t_inc;
        B_pos[0] += B_vel[0]*cosDeg(B_vel[1])*t_inc;
        B_pos[1] += B_vel[0]*sinDeg(B_vel[1])*t_inc;
        
        if (timeStop == "t"){
            // find the supposed positions of the balls 5 increments after the current frame
            double NEW_A_pos[] = {A_pos[0] + A_vel[0]*cosDeg(A_vel[1])*5*t_inc,  A_pos[1] + A_vel[0]*sinDeg(A_vel[1])*5*t_inc};
            double NEW_B_pos[] = {B_pos[0] + B_vel[0]*cosDeg(B_vel[1])*5*t_inc, B_pos[1] + B_vel[0]*sinDeg(B_vel[1])*5*t_inc};
            
            // if the future positions are overlapping, then start printing
            if (isColliding(NEW_A_pos, NEW_B_pos, A_rad, B_rad) == true){
                printStats(t_ran, A_pos, A_vel, B_pos, B_vel, velPrinting, isColliding(A_pos, B_pos, A_rad, B_rad));
            }
            // if the future positions are not overlapping
            else{
                // if the collision already did happen, this means the current frame is right after the collision
                if (collisionDidHappen == true){
                    // then print a final amount of frames for objects after collision, and quit program
                    for (int endFrame = 0; endFrame < 5; endFrame += 1){
                        double NEW_A_pos[] = {A_pos[0] + A_vel[0]*cosDeg(A_vel[1])*endFrame*t_inc,  A_pos[1] + A_vel[0]*sinDeg(A_vel[1])*endFrame*t_inc};
                        double NEW_B_pos[] = {B_pos[0] + B_vel[0]*cosDeg(B_vel[1])*endFrame*t_inc, B_pos[1] + B_vel[0]*sinDeg(B_vel[1])*endFrame*t_inc};
                        printStats(t_ran, NEW_A_pos, A_vel, NEW_B_pos, B_vel, velPrinting, isColliding(NEW_A_pos, NEW_B_pos, A_rad, B_rad));
                    }
                    return 0;
                }
            }
        }

        else{
            // only print updated stats when desired
            if (fmod(t_ran,1.0000000/updatesPerSec) < t_inc)
                printStats(t_ran, A_pos, A_vel, B_pos, B_vel, velPrinting, isColliding(A_pos, B_pos, A_rad, B_rad)); // print initial positions/vel
        }
    }
    return 0;
}
