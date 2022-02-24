#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>

class Track
{
public:
    std::string title;
    std::tm dateCreat;
    int recordDurat;
};

class Player 
{
public: 
    Track tracks[3];
    
    void CorrectInputRecName(std::string& recName) { 
        bool found;
        do {
            std::cout << ":";
            std::cin >> recName;
            found = false;
            for (int i = 0; i < 3 && !found; i++) 
                if (recName == tracks[i].title) found = true;
            if (!found)
                std::cout << "Invalid audio recording name, please try again.\n";
        } while (!found);
    }
    
    void Display (int& i, std::vector<Track>& statusPlay) {
        std::stringstream text;
        char temp[80];
        strftime (temp, 80, "%Y/%m/%d", &tracks[i].dateCreat);
        text << "Name = " << tracks[i].title << ", Date of creation = " << temp << ", Recording duration = " << tracks[i].recordDurat << " seconds.";
        std::cout << text.str() << '\n';

        statusPlay.push_back(tracks[i]);
    }

    void Play(std::vector<Track>& statusPlay) {
        if (!statusPlay.empty()) 
            return;
        std::cout << "List of audio titles:\n";
        for (int i = 0; i < 3; i++)
            std::cout << tracks[i].title << '\n';
        std::cout << "Select an audio recording to play:\n";
        std::string recName;
        CorrectInputRecName(recName);
        bool found = false;
        for (int i = 0; i < 3 && !found; i++) 
            if (recName == tracks[i].title) {
                found = true;
                Display(i, statusPlay);
            }
    }

    void Pause(std::vector<Track>& statusPaused, std::vector<Track>& statusPlay) {
        if (statusPlay.empty() || !statusPaused.empty())
            return;
        statusPaused.push_back(statusPlay[0]);
        std::cout << "Pause recording.\n";
    }
    
    void Stop(std::vector<Track>& statusAudioPaused, std::vector<Track>& statusPlay) {
        if (statusPlay.empty()) 
            return;
        statusPlay.clear();
        statusAudioPaused.clear();
        std::cout << "Stop current playback.\n";
    }
    
    void Next(std::vector<Track>& statusAudioPaused, std::vector<Track>& statusPlay) {
        if (statusPlay.empty()) 
            return;
        std::srand(std::time(nullptr));
        int i;
        do {
            i = std::rand() % 3;
        } while (tracks[i].title == statusPlay[0].title &&
                tracks[i].dateCreat.tm_year == statusPlay[0].dateCreat.tm_year && 
                tracks[i].dateCreat.tm_mon == statusPlay[0].dateCreat.tm_mon && 
                tracks[i].dateCreat.tm_mday == statusPlay[0].dateCreat.tm_mday &&
                tracks[i].recordDurat == statusPlay[0].recordDurat);
        Stop(statusAudioPaused, statusPlay);
        Display(i, statusPlay);
    }
    
    void CorrectInputCommand(std::string& command) { 
        do {
            std::cout << ":";
            std::cin >> command;
            if (command != "play" && command != "pause" && command != "next" && command != "stop" && command != "exit")
                std::cout << "Invalid command, please try again.\n";
        } while (command != "play" && command != "pause" && command != "next" && command != "stop" && command != "exit");
    }

    std::string setStr(std::string& str) {
        std::time_t t = std::time(nullptr);
        std::tm local = *std::localtime(&t);
        std::string strYear = "", strMon = "", strDay = "";
        for (int i = 0; i < 4; i++) strYear += str[i]; 
        for (int i = 5; i < 7; i++) strMon += str[i]; 
        for (int i = 8; i < str.length(); i++) strDay += str[i]; 
        if (std::stoi(strYear) > local.tm_year + 1900) strYear = std::to_string(local.tm_year + 1900);//корректировка года
        if (std::stoi(strMon) > 12) strMon = "12";//корректировка месяца
        int tempMon = std::stoi(strMon);
        if ((tempMon == 01 || tempMon == 03 || tempMon == 05 || tempMon == 07 || tempMon == 8 || tempMon == 10 || tempMon == 12) && std::stoi(strDay) > 31) strDay = "31";//корректировка 31 дня
        else if ((tempMon == 04 || tempMon == 06 || tempMon == 9 || tempMon == 11) && std::stoi(strDay) > 30) strDay = "30";//корректировка 30 дня
        if ((std::stoi(strYear) % 400 == 0 || (std::stoi(strYear) % 4 == 0 && std::stoi(strYear) % 100 != 0)) && std::stoi(strDay) > 29) strDay = "29";//февраль високосный
        else if (std::stoi(strDay) > 28) strDay = "28";//февраль невисокосный

        if (std::stoi(strYear) == local.tm_year + 1900 && std::stoi(strMon) > local.tm_mon + 1) 
            strMon = std::to_string(local.tm_mon + 1);
        
        if (std::stoi(strYear) == local.tm_year + 1900 && std::stoi(strMon) == local.tm_mon + 1 && std::stoi(strDay) > local.tm_mday) 
            strDay = std::to_string(local.tm_mday);

        str = strYear + '/' + strMon + '/' + strDay;
        return str;
    }

}; Player* audioPlayer = new Player();

int main () {
    std::time_t* t = new std::time_t(std::time(nullptr));
    std::tm* local = new std::tm(*std::localtime(t));
    std::stringstream ss; 
    std::string* str = new std::string("2023:13|35");
    ss << audioPlayer->setStr(*str);
    delete str; str = nullptr;
    ss >> std::get_time(local, "%Y/%m/%d"); 
    Track* Rec1 = new Track {"audioRec1", *local, 34};
    audioPlayer->tracks[0] = *Rec1; 
    delete Rec1; Rec1 = nullptr; 
    ss.str(""); ss.clear();

    str = new std::string("2021/03/03");
    ss << audioPlayer->setStr(*str);
    delete str; str = nullptr;
    ss >> std::get_time(local, "%Y/%m/%d");   
    Track* Rec2 = new Track {"audioRec2", *local, 17};
    audioPlayer->tracks[1] = *Rec2; 
    delete Rec2; Rec2 = nullptr; 
    ss.str(""); ss.clear(); 

    str = new std::string("2020/02/02");
    ss << audioPlayer->setStr(*str);
    delete str; str = nullptr;
    ss >> std::get_time(local, "%Y/%m/%d"); 
    Track* Rec3 = new Track {"audioRec3", *local, 58};
    audioPlayer->tracks[2] = *Rec3; 
    delete Rec3; Rec3 = nullptr;
    ss.str(""); ss.clear();

    delete local, t;
    local = nullptr; t = nullptr;

    std::string* command = new std::string;
    std::vector<Track>* statusPlay = new std::vector<Track>;
    std::vector<Track>* statusPaused = new std::vector<Track>;

    while (*command != "exit") {
        std::cout << "Choose command(play/pause/next/stop/exit).\n";
        audioPlayer->CorrectInputCommand(*command);
        if (*command == "play") 
            audioPlayer->Play(*statusPlay);
        else if (*command == "pause")
            audioPlayer->Pause(*statusPaused, *statusPlay);
        else if (*command == "next") 
            audioPlayer->Next(*statusPaused, *statusPlay);
        else if (*command == "stop")
            audioPlayer->Stop(*statusPaused, *statusPlay);
    }
    delete statusPaused, statusPlay, audioPlayer, command;
    statusPaused = nullptr; statusPlay = nullptr; audioPlayer = nullptr; command = nullptr;
}