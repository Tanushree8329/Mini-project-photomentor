
#include "httplib.h"
#include "json.hpp"

#include <iostream>
#include <string>

using namespace std;
using json = nlohmann::json;

int main()
{
    httplib::Server server;

    // Send a welcome message
    server.Get("/api/hello", [](const httplib::Request& req,
                                httplib::Response& res)
    {
        json response;
        response["message"] = "Welcome to PhotoMentor!";
        res.set_content(response.dump(4), "application/json");
    });

    // Provide photography lessons
    server.Get("/api/lessons", [](const httplib::Request& req,
                                  httplib::Response& res)
    {
        json lessons = json::array();

        lessons.push_back({
            {"title", "Aperture"},
            {"explanation", "Aperture controls the size of the lens opening. A lower f-number creates a brighter image and a blurrier background."},
            {"example", "Use f/1.8 for portraits with a blurred background."}
        });

        lessons.push_back({
            {"title", "Shutter Speed"},
            {"explanation", "Shutter speed controls how long the camera sensor receives light. Fast shutter speeds freeze movement."},
            {"example", "Use 1/1000 second to freeze a moving subject."}
        });

        lessons.push_back({
            {"title", "ISO"},
            {"explanation", "ISO controls the sensor's sensitivity to light. Higher ISO helps in dark places but can add image noise."},
            {"example", "Use ISO 100 in bright daylight."}
        });

        lessons.push_back({
            {"title", "Exposure Triangle"},
            {"explanation", "Aperture, shutter speed and ISO work together to control image exposure."},
            {"example", "Balance all three settings to get the brightness you want."}
        });

        res.set_content(lessons.dump(4), "application/json");
    });

    // Serve the website and its other local files
    server.set_mount_point("/", ".");

    cout << "PhotoMentor server is starting..." << endl;
    cout << "Open http://localhost:8080/PhotoMentor.html in your browser."
         << endl;

    if (!server.listen("127.0.0.1", 8080))
    {
        cerr << "Could not start server. Check whether port 8080 is in use."
             << endl;
        return 1;
    }

    return 0;
}