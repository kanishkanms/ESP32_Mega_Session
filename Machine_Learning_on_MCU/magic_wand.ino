#include "model.h" // trained model
#include "imu.h"   // Adafruit_MPU6050 library function

// Defining all constants
#define NUM_SAMPLES 45
#define NUM_AXES 3
#define ACCEL_THRESHOLD 20
#define INTERVAL 30

float baseline[NUM_AXES];               // to calibrate the imu sensor
float features[NUM_SAMPLES * NUM_AXES]; // feature to train

// this class will be different if you used another type of classifier, just check the model.h file
Eloquent::ML::Port::RandomForest classifier; // object for RandomForest model

void setup()
{
    Serial.begin(115200); // set baud rate to 115200
    imu_setup();          // initialise imu
    calibrate();          // calibrate the sensor(IMPORTANT : you need to place on flat surface)
    Serial.println("Calibrated");
    start_screen();       // welcome message
}

// Function to calibrate the imu sensor. To calibrate correctly, place the sensor in flat surface.
void calibrate()
{
    float ax, ay, az;
    for (int i = 0; i < 10; i++)
    {
        imu_read(&ax, &ay, &az); // read accelerometer data from imu
        delay(100);
    }

    // calibration contants
    baseline[0] = ax;
    baseline[1] = ay;
    baseline[2] = az;
}

// Function to display welcome message
void start_screen()
{
    String magicstr = R"(
___  ___            _            _             _             
|  \/  |           (_)          | |           | |            
| .  . | __ _  __ _ _  ___   ___| |_ __ _ _ __| |_ ___       
| |\/| |/ _` |/ _` | |/ __| / __| __/ _` | '__| __/ __|      
| |  | | (_| | (_| | | (__  \__ \ || (_| | |  | |_\__ \_ _ _ 
\_|  |_/\__,_|\__, |_|\___| |___/\__\__,_|_|   \__|___(_|_|_)
               __/ |                                         
              |___/                                          
    )";
    Serial.println(magicstr.c_str());
    Serial.println("\r\nPredicted gestures:\n\r");
}

void loop()
{
    float ax, ay, az;
    imu_read(&ax, &ay, &az); // read accelerometer data from imu

    // calibrated values
    ax = ax - baseline[0];
    ay = ay - baseline[1];
    az = az - baseline[2];

    if (!motionDetected(ax, ay, az))
    {
        delay(10);
        return;
    }
    recordIMU(); // get imu data as feature vectors
    //printFeatures();                        // print feature vectors in serial monitor
    classify(); // predict and classify the magic gesture
    delay(2000);
    Serial.println("...");
    Serial.println("...");
    Serial.println("...");
    Serial.println("Recognising next gesture");
}

bool motionDetected(float ax, float ay, float az)
{
    return (abs(ax) + abs(ay) + abs(az)) > ACCEL_THRESHOLD;
}
void recordIMU()
{
    float ax, ay, az;

    for (int i = 0; i < NUM_SAMPLES; i++)
    {
        imu_read(&ax, &ay, &az);
        features[i * NUM_AXES + 0] = ax - baseline[0];
        features[i * NUM_AXES + 1] = ay - baseline[1];
        features[i * NUM_AXES + 2] = az - baseline[2];
        delay(INTERVAL);
    }
}
void printFeatures()
{
    const uint16_t numFeatures = sizeof(features) / sizeof(float);
    for (int i = 0; i < numFeatures; i++)
    {
        Serial.print(features[i]);
        Serial.print(i == numFeatures - 1 ? '\n' : ',');
    }
}
void classify()
{
    Serial.print("Detected gesture: ");

    // call the function predictLabel passing features as arguments in model.h using classifier object
    Serial.println(classifier.predictLabel(features));
    if (classifier.predictLabel(features) == "W")
    {
        Serial.println(
            "\n\r*         *         *\n\r *       * *       "
            "*\n\r  *     *   *     *\n\r   *   *     *   *\n\r    * *       "
            "* *\n\r     *         *\n\r\n\r");
        Serial.println("\n\r");
        Serial.println("╔══════════════════════╗");
        Serial.println("║  Wingardium Leviosa  ║");
        Serial.println("╚══════════════════════╝\n\r");
    }
    else if (classifier.predictLabel(features) == "L")
    {
        Serial.println(
            "\n\r        *\n\r       *\n\r      *\n\r     *\n\r    "
            "*\n\r   *\n\r  *\n\r * * * * * * * *\n\r");

        Serial.println("\n\r");
        Serial.println("╔══════════════════════╗");
        Serial.println("║         Lumos        ║");
        Serial.println("╚══════════════════════╝\n\r");
    }
    else if (classifier.predictLabel(features) == "O")
    {
        Serial.println(
            "\n\r          *\n\r       *     *\n\r     *         *\n\r "
            "   *           *\n\r     *         *\n\r       *     *\n\r      "
            "    *\n\r");

        Serial.println("\n\r");
        Serial.println("╔══════════════════════╗");
        Serial.println("║       Obliviate      ║");
        Serial.println("╚══════════════════════╝\n\r");
    }
}
