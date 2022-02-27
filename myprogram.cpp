#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>

class Track
{
    const char* title;
    std::tm dateCreat;
    int recordDurat;
public:
    //-------------seters--------------------
    void setTitle(const char* sTitle) {
        title = sTitle;
    }
    void setDateCreat(std::tm sDateCreat) {
        dateCreat = sDateCreat;
    }
    void setRecordDurat(int sRecordDurat) {
        recordDurat = sRecordDurat;
    }
    //-------------geters--------------------
    std::string getTitle() {
        return title;
    }
    std::string getDateCreat() {
        char temp[80];
        strftime (temp, 80, "%Y/%m/%d", &dateCreat);
        return temp;
    }
    int getRecordDurat() {
        return recordDurat;
    }
};

class Player 
{
    std::vector<Track> tracks;
    bool statusPlay = false;
    bool statusPaused = false;
    int numberCurrentAudioRecording;   
    
    void CorrectInputRecName(std::string& recName) { 
        bool found;
        do {
            std::cout << ":";
            std::cin >> recName;
            found = false;
            for (int i = 0; i < tracks.size() && !found; i++) 
                if (recName == tracks[i].getTitle()) found = true;
            if (!found)
                std::cout << "Invalid audio recording name, please try again.\n";
        } while (!found);
    }
    
    void Display (int& i) {
        std::stringstream text;
        text << "Name = " << tracks[i].getTitle() << ", Date of creation = " << tracks[i].getDateCreat() << ", Recording duration = " << tracks[i].getRecordDurat() << " seconds.";
        std::cout << text.str() << '\n';
    }

    int Play() {
        if (statusPlay) return 0;
        else statusPlay = true;
        
        std::cout << "List of audio titles:\n";
        for (int i = 0; i < tracks.size(); i++)
            std::cout << tracks[i].getTitle() << '\n';
        std::cout << "Select an audio recording to play:\n";
        std::string recName;
        CorrectInputRecName(recName);
        bool found = false;
        int i;
        for (i = 0; i < tracks.size() && !found; i++) 
            if (recName == tracks[i].getTitle()) {
                found = true;
                Display(i); 
            } 
        i--; 
        return i;
    }

    void Pause() {
        if (!statusPlay || statusPaused) return;
        else statusPaused = true;
        std::cout << "Pause recording.\n";
    }

    void Stop() {
        if (!statusPlay) return;
        else {
            statusPlay = false;
            statusPaused = false;
        }
        std::cout << "Stop current playback.\n";
    }
    
    void Next() { 
        if (!statusPlay) return;
        std::srand(std::time(nullptr));
        int i;
        do {
            i = std::rand() % 3; 
        } while (i == numberCurrentAudioRecording);
        Stop();
        numberCurrentAudioRecording = i;                                                                                                                                        
        statusPlay = true;
        Display(numberCurrentAudioRecording);
    }

    void CorrectInputCommand(std::string& command) { 
        do {
            std::cout << ":";
            std::cin >> command;
            if (command != "play" && command != "pause" && command != "next" && command != "stop" && command != "exit")
                std::cout << "Invalid command, please try again.\n";
        } while (command != "play" && command != "pause" && command != "next" && command != "stop" && command != "exit");
    }

    std::string CorrectInputDate(std::string text) {
        std::time_t t = std::time(nullptr);
        std::tm local = *std::localtime(&t);
        std::string strYear = "", strMon = "", strDay = "";
        for (int i = 0; i < 4; i++) strYear += text[i]; 
        for (int i = 5; i < 7; i++) strMon += text[i]; 
        for (int i = 8; i < text.length(); i++) strDay += text[i]; 
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

        text = strYear + '/' + strMon + '/' + strDay;
        return text;
    }
public:
    void setTracks(Track sTracks) {
        tracks.push_back(sTracks);
    }

    void setPlay() {
        Play();
    }

    void setPause() {
        Pause();
    }

    void setStop() {
        Stop();
    }

    void setNext() {
        Next();
    }
    
    void setCorrectInputCommand(std::string& command) {
        CorrectInputCommand(command);
    }
    
    std::string setCorrectInputDate(std::string text) {
        return CorrectInputDate(text);
    }
}; 

int main () {
    Player* audioPlayer = new Player();
    std::time_t t = std::time(nullptr);
    std::tm local = *std::localtime(&t);
    std::stringstream ss; 
    
    ss << audioPlayer->setCorrectInputDate("2023:13|35");
    ss >> std::get_time(&local, "%Y/%m/%d"); 
    Track* Rec1 = new Track; Rec1->setTitle("audioRec1"); Rec1->setDateCreat(local); Rec1->setRecordDurat(34);
    audioPlayer->setTracks(*Rec1);
    delete Rec1; Rec1 = nullptr; 
    ss.str(""); ss.clear();

    ss << audioPlayer->setCorrectInputDate("2021/03/03");
    ss >> std::get_time(&local, "%Y/%m/%d");   
    Track* Rec2 = new Track; Rec2->setTitle("audioRec2"); Rec2->setDateCreat(local); Rec2->setRecordDurat(17);
    audioPlayer->setTracks(*Rec2);
    delete Rec2; Rec2 = nullptr; 
    ss.str(""); ss.clear(); 

    ss << audioPlayer->setCorrectInputDate("2020/02/02");
    ss >> std::get_time(&local, "%Y/%m/%d"); 
    Track* Rec3 = new Track; Rec3->setTitle("audioRec3"); Rec3->setDateCreat(local); Rec3->setRecordDurat(58);
    audioPlayer->setTracks(*Rec3);
    delete Rec3; Rec3 = nullptr;
    ss.str(""); ss.clear();

    std::string* command = new std::string;

    while (*command != "exit") {
        std::cout << "Choose command(play/pause/next/stop/exit).\n";
        audioPlayer->setCorrectInputCommand(*command);
        if (*command == "play") 
            audioPlayer->setPlay();
        else if (*command == "pause")
            audioPlayer->setPause();
        else if (*command == "next") 
            audioPlayer->setNext();
        else if (*command == "stop")
            audioPlayer->setStop();
    }
    delete audioPlayer; audioPlayer = nullptr;
    delete command; command = nullptr;
}