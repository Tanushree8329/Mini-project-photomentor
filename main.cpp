#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct Lesson {
    string title;
    string explanation;
    string example;
};

struct CameraAdvice {
    string situation;
    string aperture;
    string shutterSpeed;
    string iso;
    string tip;
};

struct QuizQuestion {
    string question;
    vector<string> options;
    int correctOption;
    string explanation;
};

class PhotoMentorApp {
private:
    vector<Lesson> lessons;
    vector<CameraAdvice> cameraAdvice;
    vector<string> exercises;
    vector<QuizQuestion> quizQuestions;
    set<int> completedExercises;
    vector<int> quizScores;
    const string progressFile = "photomentor_progress.txt";

    int readChoice(const string& prompt, int minValue, int maxValue) {
        while (true) {
            cout << prompt;
            string line;

            if (!getline(cin, line)) {
                throw runtime_error("Input stream closed.");
            }

            stringstream input(line);
            int choice;
            char extra;

            if ((input >> choice) && !(input >> extra) &&
                choice >= minValue && choice <= maxValue) {
                return choice;
            }

            cout << "Please enter a whole number from "
                 << minValue << " to " << maxValue << ".\n";
        }
    }

    void setupLessons() {
        lessons = {
            {
                "Aperture",
                "Aperture is the adjustable opening inside a camera lens that "
                "controls how much light enters the camera. It is measured in "
                "f-numbers, such as f/1.8, f/4, f/8 and f/11. A smaller f-number "
                "means a wider opening, which lets in more light and can blur "
                "the background. A larger f-number means a narrower opening "
                "and can keep more of the scene in focus. Aperture is especially "
                "useful for controlling background blur in portraits and "
                "sharpness throughout landscapes.",
                "Photograph a flower using a wide aperture if your camera "
                "supports it. Then use f/8 or f/11 and compare how much of "
                "the background appears sharp."
            },
            {
                "Shutter Speed",
                "Shutter speed describes how long the camera sensor receives "
                "light for a photograph. A fast shutter speed, such as "
                "1/1000 second, can freeze moving people, animals or sports. "
                "A slower shutter speed can show movement as blur, which can "
                "be useful for waterfalls or moving lights. However, a slow "
                "shutter can also make the whole image blurry if the camera "
                "moves during the exposure. Choose your shutter speed based "
                "on whether you want to freeze movement or show it creatively.",
                "Photograph a moving person using a fast shutter speed. "
                "If your camera allows it, try a slower speed with the camera "
                "held steadily and compare the movement."
            },
            {
                "ISO",
                "ISO is a camera setting that affects the brightness of the "
                "captured image. A lower ISO, such as ISO 100, generally gives "
                "cleaner images with less visible digital noise. A higher ISO "
                "can help you photograph in darker places without using an "
                "extremely slow shutter speed. The trade-off is that higher "
                "ISO can make grain or digital noise more noticeable. Start "
                "with a low ISO in bright light and increase it when the "
                "available light is not enough.",
                "Take one photograph at ISO 100 and another at ISO 800 in "
                "similar conditions, if your camera supports manual ISO. "
                "Compare the brightness and visible noise."
            },
            {
                "Exposure Triangle",
                "The exposure triangle consists of aperture, shutter speed "
                "and ISO. These three settings work together to control the "
                "brightness of a photograph. Aperture affects both light and "
                "background blur, shutter speed affects light and motion, "
                "and ISO affects brightness and image noise. Changing one "
                "setting may require changing another to keep a similar "
                "brightness. Learning how they interact helps you make "
                "intentional choices instead of relying only on automatic mode.",
                "Photograph the same subject twice. Change the shutter speed "
                "and adjust aperture or ISO, if available, to keep the "
                "brightness similar. Compare the motion and background blur."
            },
            {
                "Composition and Rule of Thirds",
                "Composition means arranging the subject and other visual "
                "elements inside a photograph. The rule of thirds divides "
                "the frame into nine equal sections using two horizontal "
                "and two vertical lines. Placing a subject near these lines "
                "or their intersections can create a balanced and interesting "
                "image. Other composition techniques include leading lines, "
                "symmetry, framing and negative space. These are useful "
                "guidelines, not strict rules, so centre a subject whenever "
                "that better communicates your idea.",
                "Enable the grid on your phone camera if available. Photograph "
                "a person or object in the centre, then near a rule-of-thirds "
                "intersection. Compare which framing works better."
            },
            {
                "Focus",
                "Focus determines which part of a photograph appears sharp. "
                "Autofocus allows the camera to focus automatically, while "
                "manual focus lets you adjust focus yourself on supported "
                "cameras. For portraits, the eyes are often the most important "
                "place to focus because sharp eyes make a face look clearer. "
                "For moving subjects, continuous autofocus can help keep "
                "tracking the subject. On a phone, tapping the subject on "
                "the screen often tells the camera where to focus.",
                "Take a portrait and zoom in to check whether the eyes are "
                "sharp. On your phone, tap the person's face before taking "
                "another photograph."
            },
            {
                "Lighting",
                "Lighting affects the brightness, colours, shadows, texture "
                "and mood of a photograph. Front lighting illuminates the "
                "subject from the camera's direction and can make details "
                "easy to see. Side lighting creates shadows that reveal "
                "shape and texture, while backlighting comes from behind "
                "the subject and can create a glowing outline. Soft light "
                "creates gentle shadows, while hard light creates stronger "
                "shadows. Window light, open shade and sunrise or sunset "
                "are useful places to practise observing light.",
                "Photograph the same object in front light and side light. "
                "Compare how the direction and softness of the light change "
                "its appearance."
            },
            {
                "White Balance",
                "White balance adjusts the colour appearance of a photograph "
                "to account for the colour of the light source. Without a "
                "suitable setting, a white object may look too orange, yellow "
                "or blue. Common settings include Auto, Daylight, Cloudy, "
                "Shade and Tungsten, although the options depend on your "
                "device. Auto white balance works well in many situations, "
                "but choosing a preset can give more consistent results. "
                "White balance can also be adjusted creatively to make a "
                "scene look warmer or cooler.",
                "Photograph the same scene using Auto white balance and a "
                "suitable preset, if available. Compare the colours in the "
                "two images."
            },
            {
                "Focal Length",
                "Focal length is measured in millimetres on interchangeable "
                "camera lenses and affects the angle of view. A wide-angle "
                "lens includes more of the scene, which can be useful for "
                "landscapes and small rooms. A longer focal length frames "
                "a narrower view and can help photograph distant subjects. "
                "Different phone cameras may have ultrawide, main and "
                "telephoto lenses. Remember that zooming or changing lenses "
                "changes framing, while moving the camera changes perspective.",
                "Use your phone's ultrawide, main and zoom cameras if available. "
                "Photograph the same subject with each and compare the "
                "amount of the scene visible."
            },
            {
                "Depth of Field",
                "Depth of field is the range of distances that appear "
                "acceptably sharp in a photograph. A shallow depth of field "
                "keeps the subject sharp while making the background look "
                "blurred. A deeper depth of field keeps more of the scene "
                "sharp and is often useful for landscapes. Aperture, focal "
                "length, subject distance and sensor size all influence "
                "depth of field. Phones may simulate background blur using "
                "software, so the effect can differ from a camera with "
                "a real optical shallow depth of field.",
                "Place a nearby object in front of a distant background. "
                "Photograph it with the background blurred, then try to "
                "keep both the object and background sharp."
            },
            {
                "HDR Photography",
                "HDR stands for High Dynamic Range. It helps preserve detail "
                "in very bright and very dark parts of a scene when one "
                "ordinary exposure cannot capture both well. Many phones "
                "automatically combine or process image information to "
                "produce an HDR result. HDR can be useful when photographing "
                "a bright sky above a shaded foreground. It may produce "
                "ghosting or unnatural-looking results if subjects move "
                "or processing is too strong.",
                "Photograph a scene containing a bright sky and a darker "
                "foreground with HDR enabled and disabled, if your device "
                "offers that option. Compare the detail in the sky and shadows."
            },
            {
                "Portrait Photography and Portrait Mode",
                "Portrait photography is about making a person the main "
                "subject of an image. Good portraits usually need careful "
                "focus on the eyes, suitable light, thoughtful framing and "
                "a background that does not distract from the person. Phone "
                "Portrait mode often uses software to simulate background "
                "blur, while some cameras can create optical blur using "
                "aperture, lens and subject distance. Keep enough distance "
                "between the person and the background to help separation. "
                "Check hair edges and glasses because simulated blur can "
                "sometimes look inaccurate.",
                "Take a portrait near a window using soft light. Focus on "
                "the eyes, move the person away from a busy background, "
                "and compare Portrait mode with a normal photo if available."
            }
        };
    }

    void setupCameraAdvice() {
        cameraAdvice = {
            {
                "Portrait Photography",
                "f/1.8 to f/4, if available",
                "Around 1/125 second or faster",
                "Start low; raise if needed",
                "Focus on the eyes. Use soft light and a simple background. "
                "A wider aperture can blur the background, but make sure "
                "the eyes remain sharp."
            },
            {
                "Landscape Photography",
                "Around f/8 to f/11, if available",
                "Use a speed that avoids camera shake",
                "Use a low ISO when practical",
                "Look for foreground interest, leading lines and a clear "
                "horizon. A tripod can help when light is low. Phone cameras "
                "may not offer direct aperture control."
            },
            {
                "Moving Subjects",
                "Choose an aperture that gives enough light",
                "Around 1/500 second or faster",
                "Raise ISO if the image is too dark",
                "Fast action may need 1/1000 second or faster. Use continuous "
                "autofocus if available and follow the subject as it moves."
            },
            {
                "Low-Light Photography",
                "Use a wider aperture if available",
                "Balance speed against camera shake",
                "Increase ISO if necessary",
                "Look for window light or other available light. Stabilize "
                "the camera for stationary subjects, and remember that "
                "moving people can blur during long exposures."
            },
            {
                "Night Photography",
                "Start around f/2.8 to f/5.6, if available",
                "Use a tripod for longer exposures",
                "Adjust based on the scene",
                "Longer exposures can capture more light but may blur moving "
                "subjects. Keep the camera stable and experiment with "
                "exposure time and ISO."
            }
        };
    }

    void setupExercises() {
        exercises = {
            "Rule of Thirds: Photograph a subject away from the centre. "
            "Compare it with a centred composition.",

            "Leading Lines: Find a road, railing or pathway that guides "
            "the viewer's eye towards your subject.",

            "Light and Shadow: Photograph an object in front light and "
            "side light. Compare the shadows and texture.",

            "Background Blur: Photograph a nearby object with a distant "
            "background. Try creating separation between them.",

            "Freeze Motion: Photograph a moving subject using a fast "
            "shutter speed if your camera supports it.",

            "Reflection: Photograph a reflection in a mirror, window "
            "or still water. Look for symmetry and patterns.",

            "Perspective: Photograph the same object from three angles: "
            "eye level, a low angle and a high angle.",

            "White Balance: Compare Auto white balance with another "
            "available setting and observe the colour differences.",

            "Portrait Practice: Photograph a person using suitable light, "
            "focus on the eyes and check the background."
        };
    }

    void setupQuiz() {
        quizQuestions = {
            {
                "Which setting controls how long light reaches the sensor?",
                {"ISO", "Shutter speed", "White balance", "Focal length"},
                1,
                "Shutter speed controls the duration of the exposure. "
                "Fast shutter speeds help freeze movement."
            },
            {
                "Which aperture generally creates a shallower depth of field?",
                {"f/16", "f/11", "f/1.8", "f/22"},
                2,
                "A wider aperture, represented by a smaller f-number, "
                "can create a shallower depth of field."
            },
            {
                "What can happen when ISO is raised substantially?",
                {"More digital noise", "The lens becomes wider",
                 "The focal length changes", "The subject stops moving"},
                0,
                "Higher ISO can help in low light but may increase "
                "visible digital noise."
            },
            {
                "For a moving dancer, which is a useful starting point?",
                {"A very slow shutter speed", "A fast shutter speed",
                 "Changing only white balance", "Defocusing the eyes"},
                1,
                "A fast shutter speed helps freeze movement and reduce "
                "motion blur."
            },
            {
                "What does composition describe?",
                {"How elements are arranged in the frame",
                 "The battery level", "The file format only", "The camera brand"},
                0,
                "Composition is the arrangement of subjects and visual "
                "elements within the photograph."
            },
            {
                "What does white balance mainly affect?",
                {"Colour cast", "Shutter duration", "Focal length",
                 "Memory-card size"},
                0,
                "White balance helps colours look more natural under "
                "different light sources."
            },
            {
                "Which technique can help keep a bright sky and darker "
                "foreground detailed?",
                {"HDR", "Digital zoom only", "Defocus",
                 "Always use the highest ISO"},
                0,
                "HDR is designed to preserve detail across a wider "
                "range of brightness."
            },
            {
                "For a portrait, what is often a useful focus target?",
                {"The person's eyes", "The far wall every time",
                 "The brightest lamp", "The corner of the frame"},
                0,
                "Sharp eyes often help a portrait feel clear and engaging."
            }
        };
    }

    void loadProgress() {
        ifstream file(progressFile);
        if (!file) return;

        string type;
        while (file >> type) {
            if (type == "EXERCISE") {
                int exerciseNumber;
                if (file >> exerciseNumber && exerciseNumber >= 1 &&
                    exerciseNumber <= static_cast<int>(exercises.size())) {
                    completedExercises.insert(exerciseNumber);
                }
            } else if (type == "QUIZ") {
                int score;
                if (file >> score && score >= 0 &&
                    score <= static_cast<int>(quizQuestions.size())) {
                    quizScores.push_back(score);
                }
            } else {
                string restOfLine;
                getline(file, restOfLine);
            }
        }
    }

    void saveProgress() const {
        ofstream file(progressFile);
        if (!file) {
            cout << "Warning: progress could not be saved. "
                 << "Check folder permissions.\n";
            return;
        }

        for (int exerciseNumber : completedExercises) {
            file << "EXERCISE " << exerciseNumber << '\n';
        }

        for (int score : quizScores) {
            file << "QUIZ " << score << '\n';
        }
    }

    void showMenu() const {
        cout << "\n==================================================\n";
        cout << "                 PHOTOMENTOR\n";
        cout << "       Your Photography Learning Assistant\n";
        cout << "==================================================\n";
        cout << "1. Learn Photography Basics\n";
        cout << "2. Camera Settings Advisor\n";
        cout << "3. Practical Photography Exercises\n";
        cout << "4. Take a Photography Quiz\n";
        cout << "5. View Learning Progress\n";
        cout << "6. Photography Tips\n";
        cout << "0. Save and Exit\n";
        cout << "--------------------------------------------------\n";
    }

    void showLessons() {
        while (true) {
            cout << "\n=============== PHOTOGRAPHY LESSONS ===============\n";

            for (size_t i = 0; i < lessons.size(); ++i) {
                cout << i + 1 << ". " << lessons[i].title << '\n';
            }

            cout << "0. Return to main menu\n";

            int choice = readChoice(
                "Choose a lesson: ", 0,
                static_cast<int>(lessons.size())
            );

            if (choice == 0) return;

            const Lesson& lesson =
                lessons.at(static_cast<size_t>(choice - 1));

            cout << "\n--- " << lesson.title << " ---\n";
            cout << lesson.explanation << "\n\nExample to try:\n";
            cout << lesson.example << '\n';

            cout << "\nPress Enter to return to the lesson list...";
            string pause;

            if (!getline(cin, pause)) {
                throw runtime_error("Input stream closed.");
            }
        }
    }

    void showCameraAdvisor() {
        while (true) {
            cout << "\n=============== CAMERA SETTINGS ADVISOR ===============\n";

            for (size_t i = 0; i < cameraAdvice.size(); ++i) {
                cout << i + 1 << ". "
                     << cameraAdvice[i].situation << '\n';
            }

            cout << "0. Return to main menu\n";

            int choice = readChoice(
                "Choose a situation: ", 0,
                static_cast<int>(cameraAdvice.size())
            );

            if (choice == 0) return;

            const CameraAdvice& advice =
                cameraAdvice.at(static_cast<size_t>(choice - 1));

            cout << "\n--- Starting advice for "
                 << advice.situation << " ---\n";
            cout << "Aperture:      " << advice.aperture << '\n';
            cout << "Shutter speed: " << advice.shutterSpeed << '\n';
            cout << "ISO:           " << advice.iso << '\n';
            cout << "Tip:           " << advice.tip << '\n';
            cout << "\nThese are starting points, not fixed rules. "
                 << "Phone and camera controls vary.\n";

            cout << "\nPress Enter to return to the camera settings list...";
            string pause;

            if (!getline(cin, pause)) {
                throw runtime_error("Input stream closed.");
            }
        }
    }

    void showExercises() {
        while (true) {
            cout << "\n=============== PRACTICAL EXERCISES ===============\n";

            for (size_t i = 0; i < exercises.size(); ++i) {
                cout << i + 1 << ". ["
                     << (completedExercises.count(static_cast<int>(i + 1))
                         ? 'X' : ' ')
                     << "] " << exercises[i] << '\n';
            }

            cout << "0. Return to main menu\n";

            int choice = readChoice(
                "Choose an exercise to mark complete (0 to return): ",
                0, static_cast<int>(exercises.size())
            );

            if (choice == 0) return;

            if (completedExercises.insert(choice).second) {
                cout << "Exercise marked complete.\n";
            } else {
                cout << "That exercise is already marked complete.\n";
            }

            saveProgress();

            cout << "Press Enter to return to the exercise list...";
            string pause;

            if (!getline(cin, pause)) {
                throw runtime_error("Input stream closed.");
            }
        }
    }

    void takeQuiz() {
        while (true) {
            int score = 0;

            cout << "\n=============== PHOTOGRAPHY QUIZ ===============\n";
            cout << "Choose the correct option for each question.\n";

            for (size_t i = 0; i < quizQuestions.size(); ++i) {
                const QuizQuestion& q = quizQuestions[i];

                cout << "\nQ" << i + 1 << ". "
                     << q.question << '\n';

                for (size_t j = 0; j < q.options.size(); ++j) {
                    cout << "  " << j + 1 << ". "
                         << q.options[j] << '\n';
                }

                int answer = readChoice(
                    "Your answer: ", 1,
                    static_cast<int>(q.options.size())
                );

                if (answer - 1 == q.correctOption) {
                    ++score;
                    cout << "Correct! ";
                } else {
                    cout << "Not quite. ";
                }

                cout << q.explanation << '\n';
            }

            cout << "\nYour score: " << score
                 << "/" << quizQuestions.size() << '\n';

            if (score == static_cast<int>(quizQuestions.size())) {
                cout << "Excellent understanding. Keep practising!\n";
            } else if (score >=
                       static_cast<int>(quizQuestions.size()) / 2) {
                cout << "Good effort. Review any topics you found difficult.\n";
            } else {
                cout << "Review the lessons and try the quiz again.\n";
            }

            quizScores.push_back(score);
            saveProgress();

            cout << "\n1. Take the quiz again\n";
            cout << "0. Return to main menu\n";

            int next = readChoice("Choose an option: ", 0, 1);
            if (next == 0) return;
        }
    }

    void showProgress() const {
        cout << "\n=============== YOUR LEARNING PROGRESS ===============\n";

        cout << "Exercises completed: "
             << completedExercises.size() << "/"
             << exercises.size() << '\n';

        if (completedExercises.empty()) {
            cout << "No exercises completed yet. "
                 << "Choose one from the exercises menu.\n";
        } else {
            cout << "Completed exercise numbers: ";

            for (int number : completedExercises) {
                cout << number << ' ';
            }

            cout << '\n';
        }

        cout << "Quizzes attempted: " << quizScores.size() << '\n';

        if (!quizScores.empty()) {
            int total = 0;

            for (int score : quizScores) {
                total += score;
            }

            double average =
                static_cast<double>(total) / quizScores.size();

            cout << "Best quiz score: "
                 << *max_element(quizScores.begin(), quizScores.end())
                 << "/" << quizQuestions.size() << '\n';

            cout << "Average quiz score: " << average
                 << "/" << quizQuestions.size() << '\n';
        }

        cout << "Progress is saved in " << progressFile
             << " in the program's working folder.\n";

        cout << "\nPress Enter to return to the main menu...";
        string pause;

        if (!getline(cin, pause)) {
            throw runtime_error("Input stream closed.");
        }
    }

    void showTips() const {
        const vector<string> tips = {
            "Clean your camera lens before shooting. Fingerprints can make images look hazy.",
            "Check the background before taking a photo. Remove distracting objects when possible.",
            "Tap to focus when using a smartphone so the camera knows which subject matters.",
            "Avoid digital zoom when possible; move closer or use an optical lens if available.",
            "Experiment with different angles and distances to discover better compositions.",
            "Notice where the light is coming from and how it changes the subject.",
            "Take several shots and compare the results instead of choosing the first one.",
            "Review your photographs and choose one thing to improve next time."
        };

        cout << "\n=============== PHOTOGRAPHY TIPS ===============\n";

        for (size_t i = 0; i < tips.size(); ++i) {
            cout << i + 1 << ". " << tips[i] << '\n';
        }

        cout << "\nPress Enter to return to the main menu...";
        string pause;

        if (!getline(cin, pause)) {
            throw runtime_error("Input stream closed.");
        }
    }

public:
    PhotoMentorApp() {
        setupLessons();
        setupCameraAdvice();
        setupExercises();
        setupQuiz();
        loadProgress();
    }

    void run() {
        cout << "\nWelcome to PhotoMentor!\n";
        cout << "Learn photography one step at a time.\n";

        bool running = true;

        while (running) {
            showMenu();

            int choice = readChoice("Enter your choice: ", 0, 6);

            try {
                switch (choice) {
                    case 1: showLessons(); break;
                    case 2: showCameraAdvisor(); break;
                    case 3: showExercises(); break;
                    case 4: takeQuiz(); break;
                    case 5: showProgress(); break;
                    case 6: showTips(); break;

                    case 0:
                        saveProgress();
                        cout << "Progress saved. Thanks for using PhotoMentor!\n";
                        running = false;
                        break;

                    default: break;
                }
            } catch (const exception& e) {
                cout << "Something went wrong: " << e.what() << '\n';
            }
        }
    }
};

int main() {
    try {
        PhotoMentorApp app;
        app.run();
    } catch (const exception& e) {
        cerr << "PhotoMentor could not continue: "
             << e.what() << '\n';
        return 1;
    }

    return 0;
}
