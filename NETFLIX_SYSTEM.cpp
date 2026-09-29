/*
    NETFLIX-INSPIRED MOVIE RECOMMENDATION & SUBSCRIPTION SYSTEM
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// =========================================================
//                     DATA STRUCTURES
// =========================================================

struct Movie {
    int id;
    string name;
    string genre;
    string language;
    int year;
    double rating;      // approximate rating out of 10
    string ageRating;
    string duration;
    string description;
};

struct Plan {
    string name;
    double price;   // RM per month (fictional)
    string quality;
    string devices;
};

// =========================================================
//                  GLOBAL DATA (in-memory)
// =========================================================

vector<Movie> movies;
vector<Plan> plans;
vector<int> watchlist;      // stores movie IDs

string userName = "";
string currentPlan = "None";

const string WATCHLIST_FILE = "watchlist.txt";
const string PROFILE_FILE   = "profile.txt";

// Genre and language lists used throughout the menus
const string GENRES[] = {"Action", "Comedy", "Drama", "Horror",
                          "Documentary", "Romance", "Sci-Fi & Fantasy"};
const int GENRE_COUNT = 7;

const string LANGUAGES[] = {"English", "Japanese", "Italian",
                             "Indonesian", "Tamil", "Arabic"};
const int LANGUAGE_COUNT = 6;

// =========================================================
//                  FUNCTION DECLARATIONS
// =========================================================

void setupMovies();
void setupPlans();

void showMenu();
int askInt(string prompt, int minVal, int maxVal);
string toLower(string s);

void browseByGenre();
void browseByLanguage();
void recommendMovies();
void searchMovie();

void listMovies(vector<Movie*> list);
void showMovieDetails(Movie& m);

void addToWatchlist(Movie& m);
void viewWatchlist();

void showPlans();
void calculateCost();

void saveData();
void loadData();

// =========================================================
//                          MAIN
// =========================================================

int main() {
    setupMovies();
    setupPlans();
    loadData();

    cout << "========================================\n";
    cout << "     WELCOME TO THE NETFLIX \n";
    cout << "========================================\n";

    if (userName == "") {
        cout << "Enter your name: ";
        getline(cin, userName);
        saveData();
    }
    cout << "\nHello, " << userName << "!\n";

    bool running = true;
    while (running) {
        showMenu();
        int choice = askInt("Select an option: ", 1, 7);

        switch (choice) {
            case 1: recommendMovies(); break;
            case 2: searchMovie(); break;
            case 3: browseByGenre(); break;
            case 4: browseByLanguage(); break;
            case 5: showPlans(); break;
            case 6: viewWatchlist(); break;
            case 7:
                saveData();
                cout << "\nGoodbye, " << userName << "!\n";
                running = false;
                break;
        }
    }
    return 0;
}

// =========================================================
//                  SETUP (STARTING DATA)
// =========================================================

void setupMovies() {
    movies = {
        // ---------------- ENGLISH ----------------
        {1,  "Extraction",              "Action",   "English", 2020, 6.7, "18+",  "1h 56min",
             "A black-market mercenary is hired for a high-risk rescue mission."},
        {2,  "The Old Guard",           "Action",   "English", 2020, 6.6, "18+",  "2h 05min",
             "A group of immortal mercenaries is exposed after centuries in the shadows."},
        {3,  "Red Notice",              "Action",   "English", 2021, 6.3, "PG-13","1h 58min",
             "An FBI agent and two rival art thieves are pulled into a global chase."},
        {4,  "The Wrong Missy",         "Comedy",   "English", 2020, 5.7, "18+",  "1h 30min",
             "A man accidentally invites the wrong date on a work trip to paradise."},
        {5,  "Murder Mystery",          "Comedy",   "English", 2019, 6.0, "PG-13","1h 37min",
             "A married couple becomes the prime suspects in a murder on a yacht."},
        {6,  "Step Brothers",           "Comedy",   "English", 2008, 6.9, "18+",  "1h 38min",
             "Two grown men become resentful stepbrothers when their parents marry."},
        {7,  "The Irishman",            "Drama",    "English", 2019, 7.8, "18+",  "3h 29min",
             "A truck driver becomes a hitman entangled with organized crime figures."},
        {8,  "Marriage Story",          "Drama",    "English", 2019, 7.9, "PG-13","2h 17min",
             "A couple navigates a painful divorce that tests their family bonds."},
        {9,  "The Power of the Dog",    "Drama",    "English", 2021, 6.8, "PG-13","2h 06min",
             "Tension builds on a ranch as a domineering brother resents a new arrival."},
        {10, "Bird Box",                "Horror",   "English", 2018, 6.6, "18+",  "2h 04min",
             "A mother and her children must travel blindfolded past an unseen threat."},
        {11, "Fear Street Part One: 1994","Horror", "English", 2021, 6.2, "18+",  "1h 47min",
             "Teenagers uncover a curse connected to a string of killings in their town."},
        {12, "Gerald's Game",           "Horror",   "English", 2017, 6.5, "18+",  "1h 43min",
             "A woman is handcuffed and trapped alone after a game goes wrong."},
        {13, "Our Planet",              "Documentary","English",2019, 9.3, "G",   "50min",
             "A nature series exploring how climate change affects wildlife worldwide."},
        {14, "The Social Dilemma",      "Documentary","English",2020, 7.6, "PG-13","1h 34min",
             "Tech insiders reveal how social media platforms are designed to be addictive."},
        {15, "Tiger King",              "Documentary","English",2020, 7.5, "18+", "45min/ep",
             "A wild look at the eccentric world of big cat breeders and collectors."},
        {16, "To All the Boys I've Loved Before","Romance","English",2018,7.1,"PG-13","1h 39min",
             "A teen's secret love letters are accidentally sent to her past crushes."},
        {17, "The Kissing Booth",       "Romance",  "English", 2018, 6.1, "PG-13","2h 12min",
             "A girl's first kiss at a school booth threatens her best friendship."},
        {18, "Purple Hearts",           "Romance",  "English", 2022, 6.9, "18+",  "2h 02min",
             "A marriage of convenience between two strangers turns into real feelings."},
        {19, "The Adam Project",        "Sci-Fi & Fantasy","English",2022, 6.7, "PG-13","1h 46min",
             "A time-traveling pilot teams up with his younger self on a rescue mission."},
        {20, "They Cloned Tyrone",      "Sci-Fi & Fantasy","English",2023, 6.9, "18+", "2h 02min",
             "A trio uncovers a bizarre cloning conspiracy hidden beneath their neighborhood."},
        {21, "Spaceman",                "Sci-Fi & Fantasy","English",2024, 6.0, "PG-13","1h 47min",
             "An astronaut on a lonely deep-space mission is visited by a mysterious creature."},

        // ---------------- JAPANESE ----------------
        {22, "Kingdom",                 "Action",   "Japanese",2019, 6.7, "PG-13","2h 14min",
             "A war orphan rises through the ranks to become a legendary general."},
        {23, "Alice in Borderland",     "Action",   "Japanese",2020, 7.7, "18+",  "50min/ep",
             "Stranded friends must survive deadly games in an empty alternate Tokyo."},
        {24, "The Fable",               "Action",   "Japanese",2019, 6.9, "18+",  "2h 03min",
             "A legendary assassin is ordered to live an ordinary life for one year."},
        {25, "Zom 100: Bucket List of the Dead","Comedy","Japanese",2023, 7.2, "18+","2h 03min",
             "An overworked employee finds freedom and fun during a zombie outbreak."},
        {26, "The Makanai: Cooking for the Maiko House","Comedy","Japanese",2023,7.8,"PG","45min/ep",
             "A young cook supports trainee geisha through warm, food-filled daily life."},
        {27, "Aggretsuko",              "Comedy",   "Japanese",2018, 7.8, "PG-13","15min/ep",
             "An overworked office worker releases stress through secret death-metal karaoke."},
        {28, "First Love",              "Drama",    "Japanese",2022, 7.9, "PG-13","45min/ep",
             "Two childhood sweethearts reconnect after years apart and lost chances."},
        {29, "Call Me Chihiro",         "Drama",    "Japanese",2023, 7.0, "PG-13","2h 11min",
             "A former sex worker forms gentle bonds with lonely people in a coastal town."},
        {30, "A Family",                "Drama",    "Japanese",2024, 6.8, "18+",  "2h 16min",
             "A young man raised by a crime family confronts loyalty and consequence."},
        {31, "Re/Member",               "Horror",   "Japanese",2022, 5.6, "18+",  "1h 40min",
             "Students are trapped reliving a deadly loop tied to a decade-old tragedy."},
        {32, "Ju-On: Origins",          "Horror",   "Japanese",2020, 6.0, "18+",  "30min/ep",
             "A curse tied to a haunted house spreads across generations of victims."},
        {33, "The Village",             "Horror",   "Japanese",2023, 6.7, "18+",  "2h 06min",
             "A man returns to his rural hometown and uncovers its dark hidden secrets."},
        {34, "Tokyo Idols",             "Documentary","Japanese",2017, 6.9, "PG-13","1h 28min",
             "A look inside Japan's idol culture and the fans devoted to it."},
        {35, "Five Deadly Venoms: Behind the Scenes","Documentary","Japanese",2021,6.5,"PG","1h 10min",
             "Filmmakers and fans discuss the legacy of classic Japanese action cinema."},
        {36, "Alice in Borderland: The Making","Documentary","Japanese",2022, 7.0, "PG-13","40min",
             "Cast and crew break down how the hit survival series was created."},
        {37, "Love Like the Falling Petals","Romance","Japanese",2022, 7.2, "PG-13","1h 58min",
             "A couple faces a rare illness that steadily erases the woman's memories."},
        {38, "In Love and Deep Water",  "Romance",  "Japanese",2023, 6.8, "PG-13","1h 52min",
             "Two strangers' lives intertwine around a mysterious seaside town."},
        {39, "My Happy Marriage",       "Romance",  "Japanese",2023, 7.4, "PG-13","1h 30min",
             "A mistreated young woman finds unexpected kindness in an arranged marriage."},
        {40, "Bubble",                  "Sci-Fi & Fantasy","Japanese",2022, 7.0, "PG-13","1h 41min",
             "In a flooded Tokyo governed by strange gravity, a boy meets a mysterious girl."},
        {41, "Neon Genesis Evangelion", "Sci-Fi & Fantasy","Japanese",1995, 8.4, "18+",  "25min/ep",
             "Teen pilots defend Tokyo-3 from mysterious beings using giant biomechanical mechs."},
        {42, "Godzilla Minus One",      "Sci-Fi & Fantasy","Japanese",2023, 8.5, "PG-13","2h 05min",
             "In post-war Japan, a traumatized pilot confronts a monstrous returning threat."},

        // ---------------- ITALIAN ----------------
        {43, "My Name Is Vendetta",     "Action",   "Italian", 2022, 6.0, "18+",  "1h 40min",
             "A mother trains her daughter for revenge against the men who wronged them."},
        {44, "Robbing Mussolini",       "Action",   "Italian", 2022, 6.1, "PG-13","1h 47min",
             "A ragtag crew plots a daring heist during the chaos of wartime Italy."},
        {45, "Under the Amalfi Sun",    "Action",   "Italian", 2023, 6.3, "PG-13","1h 40min",
             "Two teens set off on a coastal adventure chasing a family treasure."},
        {46, "The Price of Family",     "Comedy",   "Italian", 2022, 6.4, "PG-13","1h 41min",
             "A debt collector's plans unravel when he meets a woman with three kids."},
        {47, "Rose Island",             "Comedy",   "Italian", 2020, 7.0, "PG-13","1h 58min",
             "An eccentric engineer builds his own independent island nation off the coast."},
        {48, "Mixed by Erry",           "Comedy",   "Italian", 2023, 7.2, "PG-13","2h 13min",
             "Three brothers build an underground bootleg-tape empire in 1980s Naples."},
        {49, "The Hand of God",         "Drama",    "Italian", 2021, 7.3, "18+",  "2h 10min",
             "A teenager in Naples grows up amid family tragedy and the arrival of a footballer."},
        {50, "The Children's Train",    "Drama",    "Italian", 2023, 6.9, "PG-13","1h 46min",
             "A boy from poverty is sent north for a better life, testing family ties."},
        {51, "On My Skin",              "Drama",    "Italian", 2018, 7.2, "18+",  "1h 40min",
             "A man's death in police custody exposes a fight for truth and justice."},
        {52, "A Classic Horror Story",  "Horror",   "Italian", 2021, 5.5, "18+",  "1h 35min",
             "Strangers on a road trip crash near a remote cabin hiding a dark ritual."},
        {53, "The Binding",             "Horror",   "Italian", 2020, 5.2, "18+",  "1h 32min",
             "A woman's new relationship is threatened by an ancient family curse."},
        {54, "Don't Kill Me",           "Horror",   "Italian", 2021, 5.7, "18+",  "1h 40min",
             "A young woman gains supernatural resilience after a violent encounter."},
        {55, "Vatican Girl: The Disappearance of Emanuela Orlandi","Documentary","Italian",2022,7.4,"PG-13","45min/ep",
             "An investigation into the decades-old disappearance of a Vatican teenager."},
        {56, "SanPa: Sins of the Savior","Documentary","Italian",2020, 7.6, "18+", "50min/ep",
             "The rise and fall of a controversial rehab community and its charismatic founder."},
        {57, "Baggio: The Divine Ponytail","Documentary","Italian",2021, 7.3, "PG", "1h 32min",
             "The story of one of Italian football's most iconic and beloved players."},
        {58, "The Tearsmith",           "Romance",  "Italian", 2024, 6.5, "PG-13","2h 05min",
             "Two orphans raised as rivals discover an unexpected bond neither expected."},
        {59, "Nuovo Olimpo",            "Romance",  "Italian", 2023, 6.6, "18+",  "2h 08min",
             "Two young men's paths keep crossing across different eras of their lives."},
        {60, "Four to Dinner",          "Romance",  "Italian", 2023, 6.2, "PG-13","1h 32min",
             "Two couples on the edge of breaking up swap partners for one revealing night."},
        {61, "Vanished into the Night", "Sci-Fi & Fantasy","Italian",2022, 5.8, "18+","1h 38min",
             "A missing-persons case spirals into something stranger than anyone expected."},
        {62, "Luna Nera",               "Sci-Fi & Fantasy","Italian",2020, 6.3, "18+","45min/ep",
             "A young woman with mysterious powers is hunted as a witch in 17th-century Italy."},
        {63, "The Man Without Gravity", "Sci-Fi & Fantasy","Italian",2019, 6.4, "PG-13","1h 40min",
             "A man born without gravity hides his strange gift from the world around him."},

        // ---------------- INDONESIAN ----------------
        {64, "The Night Comes for Us",  "Action",   "Indonesian",2018, 6.9, "18+", "2h 01min",
             "A former enforcer must fight his way through his old crime syndicate."},
        {65, "The Big 4",               "Action",   "Indonesian",2022, 5.8, "18+", "1h 59min",
             "Four elite assassins are reunited to find their mentor's killer."},
        {66, "The Shadow Strays",       "Action",   "Indonesian",2024, 6.7, "18+", "2h 27min",
             "A young assassin goes rogue to protect a boy caught in a gang war."},
        {67, "My Sassy Girl",           "Comedy",   "Indonesian",2023, 6.0, "PG-13","1h 45min",
             "A mild-mannered student falls for a chaotic, unpredictable young woman."},
        {68, "Ali & Ratu Ratu Queens",  "Comedy",   "Indonesian",2021, 6.8, "PG",  "1h 47min",
             "A teenager travels to New York to reconnect with his estranged mother."},
        {69, "The Big Choice",          "Comedy",   "Indonesian",2022, 6.1, "PG",  "1h 40min",
             "A group of friends face comic chaos while chasing very different dreams."},
        {70, "Home Sweet Loan",         "Drama",    "Indonesian",2024, 6.9, "PG-13","2h 00min",
             "A young woman juggles family pressure while saving for her first home."},
        {71, "27 Steps of May",         "Drama",    "Indonesian",2019, 7.2, "18+", "1h 50min",
             "A woman slowly heals from trauma with the help of a quiet street magician."},
        {72, "Cigarette Girl",          "Drama",    "Indonesian",2023, 7.3, "PG-13","45min/ep",
             "A dying man's search for his first love uncovers a hidden family history."},
        {73, "Impetigore",              "Horror",   "Indonesian",2019, 6.5, "18+", "1h 46min",
             "A woman returns to her ancestral village and uncovers a terrifying curse."},
        {74, "The 3rd Eye",             "Horror",   "Indonesian",2017, 5.6, "18+", "1h 40min",
             "A girl gains supernatural sight after a ritual meant to find her missing sister."},
        {75, "Sengkolo: The Catastrophe of One Suro","Horror","Indonesian",2022,5.4,"18+","1h 35min",
             "A cursed night forces a family to confront long-buried supernatural debts."},
        {76, "Ice Cold: Murder, Coffee and Jessica Wongso","Documentary","Indonesian",2023,7.0,"18+","1h 30min",
             "A notorious poisoning case that gripped and divided Indonesia is revisited."},
        {77, "All Access To Rossa 25 Shining Years","Documentary","Indonesian",2023,6.8,"PG","1h 20min",
             "A concert documentary celebrating a beloved singer's milestone career."},
        {78, "Harta Tahta Raisa",       "Documentary","Indonesian",2023, 6.9, "PG", "1h 15min",
             "An intimate look at a pop star's journey through fame and reinvention."},
        {79, "Dear David",              "Romance",  "Indonesian",2023, 6.3, "PG-13","1h 47min",
             "A student's private fantasy diary about her crush is accidentally leaked."},
        {80, "Heartbreak Motel",        "Romance",  "Indonesian",2023, 6.0, "18+",  "1h 46min",
             "Strangers cross paths at a rundown motel during their lowest romantic moments."},
        {81, "The Way I Love You",      "Romance",  "Indonesian",2023, 6.2, "PG-13","1h 50min",
             "A woman questions loyalty and love after years in a comfortable relationship."},
        {82, "24 Hours with Gaspar",    "Sci-Fi & Fantasy","Indonesian",2023,6.5,"18+","1h 47min",
             "A detective with a surreal condition races against time to solve a murder."},
        {83, "Gundala",                 "Sci-Fi & Fantasy","Indonesian",2019, 6.7, "PG-13","2h 03min",
             "A factory worker gains lightning-based powers and becomes a reluctant hero."},
        {84, "Sri Asih",                "Sci-Fi & Fantasy","Indonesian",2022, 5.9, "PG-13","2h 05min",
             "A young woman discovers she carries the spirit of an ancient warrior goddess."},

        // ---------------- TAMIL ----------------
        {85, "Leo",                     "Action",   "Tamil",   2023, 7.4, "18+",  "2h 44min",
             "A quiet café owner's violent past resurfaces when old enemies track him down."},
        {86, "Beast",                   "Action",   "Tamil",   2022, 5.6, "PG-13","2h 34min",
             "An ex-intelligence officer must rescue hostages trapped inside a mall."},
        {87, "Jailer",                  "Action",   "Tamil",   2023, 7.2, "PG-13","2h 48min",
             "A retired jailer returns to brutal form to rescue his kidnapped son."},
        {88, "Doctor",                  "Comedy",   "Tamil",   2021, 7.5, "PG-13","2h 26min",
             "A pediatrician races to save a kidnapped child while dodging comic chaos."},
        {89, "Comali",                  "Comedy",   "Tamil",   2019, 6.9, "PG-13","2h 26min",
             "A man wakes from a decade-long coma into a world he no longer recognizes."},
        {90, "Mandela",                 "Comedy",   "Tamil",   2021, 7.8, "PG",  "2h 05min",
             "A barber's swing vote turns him into the center of a village election battle."},
        {91, "Meiyazhagan",             "Drama",    "Tamil",   2024, 8.4, "PG",  "2h 26min",
             "A man returns to his hometown and reconnects with a long-forgotten relative."},
        {92, "Jai Bhim",                "Drama",    "Tamil",   2021, 8.7, "18+", "2h 44min",
             "A lawyer fights for justice after a tribal man disappears in police custody."},
        {93, "Super Deluxe",            "Drama",    "Tamil",   2019, 8.2, "18+", "2h 56min",
             "Several intertwined stories unfold over one chaotic day in Chennai."},
        {94, "Pisaasu",                 "Horror",   "Tamil",   2014, 7.5, "PG-13","1h 55min",
             "A man haunted by a spirit tries to uncover the truth behind her death."},
        {95, "Demonte Colony",          "Horror",   "Tamil",   2015, 6.9, "18+",  "2h 10min",
             "Friends spend a night in a supposedly haunted apartment complex."},
        {96, "Jackson Durai",           "Horror",   "Tamil",   2016, 5.9, "PG-13","2h 15min",
             "A ghost hunter's fake exorcisms turn real when a genuine spirit appears."},
        {97, "Nayanthara: Beyond the Fairytale","Documentary","Tamil",2024,6.7,"PG-13","1h 30min",
             "An intimate look at a leading actress's career, marriage, and motherhood."},
        {98, "Modern Masters: S.S. Rajamouli","Documentary","Tamil",2024,7.8,"PG","50min",
             "A profile of the acclaimed director behind some of Indian cinema's biggest hits."},
        {99, "96",                      "Romance",  "Tamil",   2018, 8.5, "PG-13","2h 38min",
             "Former classmates reunite at a school reunion and revisit their unfinished love."},
        {100,"Hey Sinamika",            "Romance",  "Tamil",   2022, 6.8, "PG-13","2h 15min",
             "A woman's contrasting relationships with two very different men come to light."},
        {101,"Pyaar Prema Kalyanam",    "Romance",  "Tamil",   2018, 7.0, "PG-13","2h 24min",
             "A young man's laid-back approach to love is tested by a determined woman."},
        {102,"24",                      "Sci-Fi & Fantasy","Tamil",2016, 7.4, "PG-13","2h 35min",
             "A scientist's time-controlling watch is stolen by his vengeful twin brother."},
        {103,"Indru Netru Naalai",      "Sci-Fi & Fantasy","Tamil",2015, 7.3, "PG-13","2h 15min",
             "A struggling inventor accidentally creates a working time machine."},
        {104,"Tik Tik Tik",             "Sci-Fi & Fantasy","Tamil",2018, 5.9, "PG-13","2h 08min",
             "A crew races to stop an asteroid from colliding with Earth."},

        // ---------------- ARABIC ----------------
        {105,"Mosul",                   "Action",   "Arabic",  2019, 7.2, "18+",  "1h 41min",
             "An elite Iraqi SWAT team fights to defend their city street by street."},
        {106,"Head to Head",            "Action",   "Arabic",  2022, 5.8, "PG-13","1h 30min",
             "Two rival fighters are forced to team up against a common threat."},
        {107,"The Worthy",              "Action",   "Arabic",  2016, 5.5, "18+",  "1h 32min",
             "Survivors guarding a water source must decide who deserves to stay."},
        {108,"Crashing Eid",            "Comedy",   "Arabic",  2023, 6.4, "PG",  "40min/ep",
             "A family's Eid holiday spirals into comic disaster after one bad decision."},
        {109,"The President's Cake",    "Comedy",   "Arabic",  2024, 6.9, "PG",  "1h 25min",
             "A young girl is tasked with baking a cake for a dictator's birthday party."},
        {110,"Honeymoonish",            "Comedy",   "Arabic",  2022, 6.5, "PG-13","1h 40min",
             "A couple's romantic honeymoon plans fall apart in increasingly funny ways."},
        {111,"AlRawabi School for Girls","Drama",   "Arabic",  2021, 6.9, "18+",  "40min/ep",
             "A group of students plots revenge against a classmate's relentless bullying."},
        {112,"Finding Ola",             "Drama",    "Arabic",  2022, 6.3, "PG-13","35min/ep",
             "A woman rebuilds her identity and independence after a difficult divorce."},
        {113,"The Exchange",            "Drama",    "Arabic",  2023, 6.5, "PG-13","40min/ep",
             "A sheltered woman defies her family to chase a career on the stock exchange."},
        {114,"Paranormal",              "Horror",   "Arabic",  2020, 6.9, "18+",  "45min/ep",
             "A hematologist is drawn into unsettling supernatural mysteries in 1960s Egypt."},
        {115,"The Matchmaker",          "Horror",   "Arabic",  2023, 5.7, "PG-13","1h 35min",
             "A woman's search for love takes a dark turn involving an eerie matchmaker."},
        {116,"From the Ashes",          "Horror",   "Arabic",  2023, 5.6, "18+",  "1h 38min",
             "A family confronts a vengeful presence tied to a tragedy from their past."},
        {117,"Dubai Bling",             "Documentary","Arabic",2022, 6.0, "PG-13","45min/ep",
             "A reality series following wealthy socialites navigating life in Dubai."},
        {118,"Saudi Pro League: Kickoff","Documentary","Arabic",2023, 6.3, "PG", "45min/ep",
             "An inside look at the ambitious rise of Saudi Arabia's top football league."},
        {119,"The Swimmers",            "Documentary","Arabic",2022, 7.5, "PG-13","2h 14min",
             "Two sisters flee war and swim to save a boat of refugees at sea."},
        {120,"Habibi",                  "Romance",  "Arabic",  2021, 6.1, "PG-13","1h 30min",
             "A romance blossoms between two people from very different social worlds."},
        {121,"Barakah Meets Barakah",   "Romance",  "Arabic",  2016, 6.9, "PG-13","1h 28min",
             "A municipal worker and an online star navigate courtship under strict rules."},
        {122,"Perfect Strangers",       "Romance",  "Arabic",  2021, 6.8, "18+",  "1h 40min",
             "Friends at a dinner party agree to share every text and call, with messy results."},
        {123,"Jinn",                    "Sci-Fi & Fantasy","Arabic",2019, 5.3, "18+","30min/ep",
             "Jordanian teenagers accidentally awaken an ancient supernatural spirit."},
        {124,"The Blueprint",           "Sci-Fi & Fantasy","Arabic",2023, 6.0, "PG-13","1h 35min",
             "An architect uncovers a mysterious design that seems to predict the future."}
    };
}

void setupPlans() {
    plans = {
        {"Mobile",   17.00, "SD",      "Mobile/Tablet"},
        {"Basic",    29.00, "HD",      "1 device"},
        {"Standard", 49.00, "Full HD", "2 devices"},
        {"Premium",  62.00, "4K",      "4 devices"}
    };
}

// =========================================================
//                     MENU / INPUT HELPERS
// =========================================================

void showMenu() {
    cout << "\n========================================\n";
    cout << "           NETFLIX\n";
    cout << "========================================\n";
    cout << "1. Get Movie Recommendations\n";
    cout << "2. Search for a Movie\n";
    cout << "3. Browse by Genre\n";
    cout << "4. Browse by Language\n";
    cout << "5. Subscription Plans\n";
    cout << "6. My Watchlist\n";
    cout << "7. Exit\n";
    cout << "========================================\n";
}

// Reads an integer safely; keeps asking until valid
int askInt(string prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.eof()) { saveData(); exit(0); }
        if (cin.fail() || value < minVal || value > maxVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}

string toLower(string s) {
    for (char &c : s) c = tolower(c);
    return s;
}

// =========================================================
//                  BROWSE BY GENRE / LANGUAGE
// =========================================================

void browseByGenre() {
    cout << "\nChoose a genre:\n";
    for (int i = 0; i < GENRE_COUNT; i++) cout << (i+1) << ". " << GENRES[i] << "\n";
    int g = askInt("Enter choice: ", 1, GENRE_COUNT);
    string genre = GENRES[g-1];

    vector<Movie*> results;
    for (auto &m : movies) if (m.genre == genre) results.push_back(&m);

    cout << "\nYou selected: " << genre << "\n";
    if (results.empty()) {
        cout << "No movies found in this genre.\n";
        return;
    }
    listMovies(results);
}

void browseByLanguage() {
    cout << "\nChoose a language:\n";
    for (int i = 0; i < LANGUAGE_COUNT; i++) cout << (i+1) << ". " << LANGUAGES[i] << "\n";
    int l = askInt("Enter choice: ", 1, LANGUAGE_COUNT);
    string language = LANGUAGES[l-1];

    vector<Movie*> results;
    for (auto &m : movies) if (m.language == language) results.push_back(&m);

    cout << "\nYou selected: " << language << "\n";
    if (results.empty()) {
        cout << "No movies found in this language.\n";
        return;
    }
    listMovies(results);
}

// =========================================================
//                RECOMMENDATION SYSTEM (SIMPLE & STRICT)
// =========================================================

void recommendMovies() {
    cout << "\n========================================\n";
    cout << "         MOVIE RECOMMENDATIONS\n";
    cout << "========================================\n";

    cout << "\nChoose a genre:\n";
    for (int i = 0; i < GENRE_COUNT; i++) cout << (i+1) << ". " << GENRES[i] << "\n";
    int g = askInt("Enter choice: ", 1, GENRE_COUNT);
    string genre = GENRES[g-1];

    cout << "\nChoose a language:\n";
    for (int i = 0; i < LANGUAGE_COUNT; i++) cout << (i+1) << ". " << LANGUAGES[i] << "\n";
    int l = askInt("Enter choice: ", 1, LANGUAGE_COUNT);
    string language = LANGUAGES[l-1];

    // STRICT filter: genre AND language must both match.
    // No mixing in other languages or genres.
    vector<Movie*> matches;
    for (auto &m : movies) {
        if (m.genre == genre && m.language == language) {
            matches.push_back(&m);
        }
    }

    cout << "\n========================================\n";
    cout << "Genre: " << genre << " | Language: " << language;
    cout << "\n========================================\n";

    if (matches.empty()) {
        cout << "\nSorry, no " << genre << " movies in " << language
             << " match your preferences.\n";
        return;
    }

    cout << "\nWe recommend:\n\n";
    for (size_t i = 0; i < matches.size() && i < 5; i++) {
        cout << (i+1) << ". " << matches[i]->name
             << " (Rating: " << matches[i]->rating << ")\n";
    }

    cout << "\nView details of a movie? Enter number, or 0 to go back: ";
    int pick = askInt("", 0, (int)min(matches.size(), (size_t)5));
    if (pick != 0) showMovieDetails(*matches[pick-1]);
}

// =========================================================
//                     SEARCH MOVIE
// =========================================================

void searchMovie() {
    cout << "\nEnter movie name (or part of it): ";
    string query;
    getline(cin, query);
    query = toLower(query);

    vector<Movie*> results;
    for (auto &m : movies) {
        if (toLower(m.name).find(query) != string::npos) results.push_back(&m);
    }

    if (results.empty()) {
        cout << "No movies found matching \"" << query << "\".\n";
        return;
    }
    listMovies(results);
}

void listMovies(vector<Movie*> list) {
    cout << "\n";
    for (size_t i = 0; i < list.size(); i++) {
        cout << (i+1) << ". " << list[i]->name
             << " (" << list[i]->genre << ", " << list[i]->language
             << ") - Rating: " << list[i]->rating << "\n";
    }
    cout << "\nView details? Enter number, or 0 to go back: ";
    int choice = askInt("", 0, (int)list.size());
    if (choice != 0) showMovieDetails(*list[choice-1]);
}

// =========================================================
//                     MOVIE DETAILS
// =========================================================

void showMovieDetails(Movie& m) {
    cout << "\n========================================\n";
    cout << "              MOVIE DETAILS\n";
    cout << "========================================\n";
    cout << "Name: " << m.name << "\n";
    cout << "Genre: " << m.genre << "\n";
    cout << "Language: " << m.language << "\n";
    cout << "Year: " << m.year << "\n";
    cout << "Rating: " << m.rating << "/10\n";
    cout << "Age Rating: " << m.ageRating << "\n";
    cout << "Duration: " << m.duration << "\n";
    cout << "\nDescription:\n" << m.description << "\n";
    cout << "========================================\n";

    cout << "\n1. Add to Watchlist\n2. Return\n";
    int choice = askInt("Select an option: ", 1, 2);
    if (choice == 1) addToWatchlist(m);
}

// =========================================================
//                      WATCHLIST
// =========================================================

void addToWatchlist(Movie& m) {
    for (int id : watchlist) {
        if (id == m.id) {
            cout << "\"" << m.name << "\" is already in your watchlist.\n";
            return;
        }
    }
    watchlist.push_back(m.id);
    cout << "\"" << m.name << "\" added to your watchlist.\n";
    saveData();
}

void viewWatchlist() {
    cout << "\n========================================\n";
    cout << "              MY WATCHLIST\n";
    cout << "========================================\n";
    if (watchlist.empty()) {
        cout << "Your watchlist is empty.\n";
        return;
    }
    for (size_t i = 0; i < watchlist.size(); i++) {
        for (auto &m : movies) {
            if (m.id == watchlist[i]) {
                cout << (i+1) << ". " << m.name << " (" << m.genre << ")\n";
            }
        }
    }
    cout << "\n1. Remove a movie\n2. Return to Main Menu\n";
    int choice = askInt("Select an option: ", 1, 2);
    if (choice == 1) {
        int pick = askInt("Enter movie number to remove: ", 1, (int)watchlist.size());
        watchlist.erase(watchlist.begin() + (pick - 1));
        cout << "Removed from watchlist.\n";
        saveData();
    }
}

// =========================================================
//                  SUBSCRIPTION PLANS
// =========================================================

void showPlans() {
    cout << "\n========================================\n";
    cout << "           SUBSCRIPTION PLANS\n";
    cout << "   (Simulated prices - not real Netflix pricing)\n";
    cout << "========================================\n";
    for (size_t i = 0; i < plans.size(); i++) {
        cout << (i+1) << ". " << plans[i].name
             << "\n   Monthly: RM" << fixed << setprecision(2) << plans[i].price
             << "\n   Quality: " << plans[i].quality
             << "\n   Devices: " << plans[i].devices << "\n\n";
    }
    cout << "1. Calculate subscription cost\n2. Return to Main Menu\n";
    int choice = askInt("Select an option: ", 1, 2);
    if (choice == 1) calculateCost();
}

void calculateCost() {
    cout << "\nChoose your plan:\n";
    for (size_t i = 0; i < plans.size(); i++) {
        cout << (i+1) << ". " << plans[i].name << " - RM"
             << fixed << setprecision(2) << plans[i].price << "/month\n";
    }
    int p = askInt("Enter choice: ", 1, (int)plans.size());
    Plan chosen = plans[p-1];

    int months = askInt("Enter number of months: ", 1, 60);
    double subtotal = chosen.price * months;

    int discount = askInt("Enter discount percentage (0 if none): ", 0, 100);
    double discountAmount = subtotal * discount / 100.0;
    double total = subtotal - discountAmount;

    cout << "\n========================================\n";
    cout << "                RECEIPT\n";
    cout << "========================================\n";
    cout << "Plan: " << chosen.name << "\n";
    cout << "Monthly price: RM" << fixed << setprecision(2) << chosen.price << "\n";
    cout << "Months: " << months << "\n";
    cout << "Subtotal: RM" << fixed << setprecision(2) << subtotal << "\n";
    cout << "Discount (" << discount << "%): RM" << fixed << setprecision(2) << discountAmount << "\n";
    cout << "----------------------------------------\n";
    cout << "Final Total: RM" << fixed << setprecision(2) << total << "\n";
    cout << "========================================\n";

    cout << "\nSet as your current plan? (1 = Yes, 2 = No): ";
    int set = askInt("", 1, 2);
    if (set == 1) {
        currentPlan = chosen.name;
        cout << "Your plan is now: " << currentPlan << "\n";
        saveData();
    }
}

// =========================================================
//                    FILE HANDLING (SAVE/LOAD)
// =========================================================

void saveData() {
    ofstream pf(PROFILE_FILE);
    if (pf.is_open()) {
        pf << userName << "\n" << currentPlan << "\n";
        pf.close();
    }
    ofstream wf(WATCHLIST_FILE);
    if (wf.is_open()) {
        for (int id : watchlist) wf << id << "\n";
        wf.close();
    }
}

void loadData() {
    ifstream pf(PROFILE_FILE);
    if (pf.is_open()) {
        getline(pf, userName);
        getline(pf, currentPlan);
        pf.close();
    }
    ifstream wf(WATCHLIST_FILE);
    if (wf.is_open()) {
        int id;
        while (wf >> id) watchlist.push_back(id);
        wf.close();
    }
}