#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

template <typename T> T input(string text);

template <> string input<string>(string text);

template <typename T> struct Array {
  int length;
  int capacity;
  T *data = nullptr;

  Array(int capacity = 2)
      : length(0), capacity(capacity), data(new T[capacity]) {}

  void insert(T value) {
    if (length == capacity) {
      capacity *= 2;
      T *newData = new T[capacity];

      for (int i = 0; i < length; i++) {
        newData[i] = data[i];
      }

      delete[] data;
      data = newData;
    }
    data[length++] = value;
  }

  void removeAt(int index) {
    T *newData = new T[capacity];
    for (int i = index; i < length - 1; i++) {
      newData[i] = data[i + 1];
    }

    length--;

    delete[] data;
    data = newData;

    if (capacity == 2 * length && capacity > 2) {
      capacity /= 2;
    }
  }

  T at(int index) { return data[index]; }

  void print() {
    for (int i = 0; i < length; i++) {
      cout << data[i] << "\n";
    }
  }
};

class CSV {
private:
  string *ptr = nullptr;
  Array<Array<string>> csv;
  string filePath;

public:
  CSV(string path) : filePath(path) {}

  void parse() {
    FILE *fptr;
    fptr = fopen(filePath.c_str(), "r");

    if (fptr == NULL) {
      perror("Error opening file:");
      return;
    }

    char buffer[500];

    string temp = "";
    while (fgets(buffer, 500, fptr)) {
      Array<string> *arrP = new Array<string>;
      bool isPush = false;

      for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
        if (buffer[i] == ',') {
          arrP->insert(temp);
          temp = "";
          isPush = true;
        } else {
          if (!isPush) {
            temp += buffer[i];
          }
          isPush = false;
        }
      }

      csv.insert(*arrP);
      delete arrP;
    }

    fclose(fptr);
  }

  void read() {
    if (csv.length == 0) {
      cout << "There is no data in the CSV or you haven't parsed it yet.\n";
      return;
    }

    for (int i = 0; i < csv.length; i++) {
      cout << "row ke-" << i << endl;
      for (int j = 0; j < csv.data->length; j++) {
        cout << csv.data[i].data[j] << endl;
      }
    }
  }

  Array<string> get(string field) {
    int index;
    Array<string> arr;

    for (int i = 0; i < csv.data->length; i++) {
      if (csv.data[0].data[i] == field) {
        index = i;
      }
    }

    for (int i = 1; i < csv.length; i++) {
      arr.insert(csv.data[i].data[index]);
    }

    return arr;
  }

  int length(string field = "all") {
    if (field != "all")
      return csv.length - 1;
    return csv.length;
  }
};

struct Answer {
  int id;
  int authorId;
  string text;
  int vote = 0;
  bool isAccepted = false;
};

struct Comment {
  int id;
  int authorId;
  string text;
  int vote = 0;
};

struct Question {
  int id;
  int authorId;
  string title;
  string body;
  int vote = 0;
  bool isVerified = false;
  Array<Answer> answers;
  Array<Comment> comments;

  void addAnswer(int userId, string text, bool init = true) {
    Answer input;
    int id = answers.length + 1;
    input = {id, userId, text};
    answers.insert(input);

    if (init)
      return;
    cout << "Answer added successfully.";
  }

  void addComment(int userId, string text, bool init = true) {
    Comment input;
    int id = comments.length + 1;
    input = {id, userId, text};
    comments.insert(input);

    if (init)
      return;
    cout << "Comment created successfully.";
  }
};

struct User {
  int id;
  string name;
  string password;
  string email;
  int reputation = 0;
};

struct App {
  Array<User> users;
  Array<Question> questions;

  void createUser(string name, string password, string email) {
    int id = users.length + 1;
    User newUser = {id, name, password, email};

    users.insert(newUser);
  }

  void createQuestion(int userId, string title, string body,
                      bool isInit = false) {

    int id = questions.length;
    Question input = {
        .id = id,
        .authorId = userId,
        .title = title,
        .body = body,
    };

    questions.insert(input);

    if (!isInit) {
      cout << "Question created successfully.\n";
    }
  }

  void showAllQuestions() {

    if (questions.length == 0) {
      cout << "\nNo questions in database.\n";
      return;
    }
    cout << "\nAll Questions\n";
    for (int i = 0; i < questions.length; i++) {
      cout << i + 1 << ". " << questions.data[i].title << " " << questions.data[i].vote << "\n";
    }
  }

  template <typename T> void voteUp(int userId, T &object) {
    object.vote += 1;

    cout << "Vote up successfully.";
  }

  template <typename T> void voteDown(int userId, T &object) {
    object.vote -= 1;

    cout << "Vote down successfully.";
  }

  void showAnswer(int questionId) {
    Question &question = questions.data[questionId - 1];
    cout << "\n--- Question ---\n";
    cout << question.title << endl;
    cout << "\n--- Answer ---\n";

    for (int i = 0; i < question.answers.length; i++) {
      if (questions.data[questionId - 1].answers.data[i].isAccepted) {
        cout << "Status: Verified\n";
      }
      int authorId = questions.data[questionId - 1].answers.at(i).authorId;
      cout << "Username: " << users.at(authorId - 1).name << endl;
      cout << "Answer: " << question.answers.data[i].text << endl;
    }
  }

  void showComment(int questionId) {
    Question &question = questions.data[questionId - 1];

    cout << "\n--- Comment ---\n";
    for (int i = 0; i < question.comments.length; i++) {
      cout << questions.data[questionId - 1].comments.at(i).text << endl;
    }
    cout << "\n";
  }

  void assignReputation(int userId) {
    User &user = users.data[userId - 1];
    int id = user.id;
    int totalVoted = 0;

    for (int i = 0; i < questions.length; i++) {

      if (questions.data[i].authorId == id) {
        totalVoted++;
      }

      for (int j = 0; j < questions.data[i].answers.length; j++) {
        if (questions.data[i].answers.data[j].authorId == id) {
          totalVoted++;
        }
      }
    }

    user.reputation = 10 * totalVoted;
  }

  void showProfile(int userId) {
    User user = users.data[userId - 1];
    Array<Question> q;
    Array<Answer> a;

    for (int i = 0; i < questions.length; i++) {
      for (int j = 0; j < questions.data[i].answers.length; j++) {
        if (questions.data[i].answers.data[j].authorId == userId) {
          a.insert(questions.data[i].answers.data[j]);
        }
      }
    }

    for (int i = 0; i < questions.length; i++) {
      if (questions.data[i].authorId == userId) {
        q.insert(questions.data[i]);
      }
    }

    cout << "\n";
    cout << "Username\t: " << user.name << "\n";
    cout << "Email\t\t: " << user.email << "\n";
    cout << "Reputation\t: " << user.reputation << "\n";

    if (q.length > 0) {
      cout << "--- Questions List ---\n";
      for (int i = 0; i < q.length; i++) {
        cout << "#" << i + 1 << " " << q.at(i).title << "\n";
      }
    }

    if (a.length > 0) {
      cout << "--- Answer List ---\n";
      for (int i = 0; i < a.length; i++) {
        cout << "#" << i + 1 << " " << a.at(i).text << "\n";
      }
    }
  }

  void showNewestQuestions() {

    if (questions.length == 0) {
        cout << "\nNo questions available.\n";
        return;
    }

    cout << "\n=== NEWEST QUESTIONS ===\n";

    int no = 1;

    for (int i = questions.length - 1; i >= 0; i--) {

        cout << no << ". "
             << questions.data[i].title
             << " | Votes: " << questions.data[i].vote
             << " | Answers: " << questions.data[i].answers.length
             << " | ";

        if (questions.data[i].isVerified)
            cout << "Verified";
        else
            cout << "Not Verified";

        cout << endl;

        no++;
    }
}



void showQuestionDetail(int index) {

    Question &q = questions.data[index];

    cout << "\n=========================\n";
    cout << "Title    : " << q.title << endl;
    cout << "Vote     : " << q.vote << endl;

    cout << "Verified : ";

    if(q.isVerified)
        cout << "Yes";
    else
        cout << "No";

    cout << endl;

    cout << "\nBody:\n";
    cout << q.body << endl;

    cout << "=========================\n";
}

void showAnswerList(int questionId){

    Question &q = questions.data[questionId - 1];

    cout << "\n=== ANSWERS ===\n";

    if(q.answers.length == 0){
        cout << "No answers yet.\n";
        return;
    }

    for(int i=0;i<q.answers.length;i++){

        Answer &a = q.answers.data[i];

        cout << "\nAnswer #" << i+1 << endl;

        cout << "Author : "
             << users.data[a.authorId - 1].name
             << endl;

        cout << "Vote   : "
             << a.vote
             << endl;

        cout << "Status : ";

        if(a.isAccepted)
            cout << "Verified";
        else
            cout << "Not Verified";

        cout << endl;

        cout << a.text << endl;
    }
}

void questionActions(int questionIndex, int currentUserId){

    Question &q = questions.data[questionIndex];

    char actionChoice;

    cout << "\n=== ACTIONS ===\n";
    cout << "1. Vote Up Question\n";
    cout << "2. Vote Down Question\n";
    cout << "3. Add Comment\n";
    cout << "4. Post Answer\n";
    cout << "5. Vote Answer\n";
    cout << "0. Back\n";

    cout << "Choose: ";
    cin >> actionChoice;

    switch(actionChoice){

        case '1':
            voteUp(currentUserId,q);
            break;

        case '2':
            voteDown(currentUserId,q);
            break;

        case '3':{

            string comment =
                input<string>("Comment: ");

            q.addComment(
                currentUserId,
                comment,
                false
            );

            break;
        }

        case '4':{

            string answer =
                input<string>("Answer: ");

            q.addAnswer(
                currentUserId,
                answer,
                false
            );

            break;
        }

        case '5':{

            if(q.answers.length == 0){

                cout << "No answers available.\n";
                break;
            }

            for(int i=0;i<q.answers.length;i++){

                cout << i+1
                     << ". "
                     << q.answers.data[i].text
                     << endl;
            }

            int answerChoice =
                input<int>("Choose answer: ");

            if(answerChoice < 1 ||
               answerChoice > q.answers.length){

                cout << "Invalid answer.\n";
                break;
            }

            cout << "1. Vote Up\n";
            cout << "2. Vote Down\n";

            int voteChoice =
                input<int>("Choose: ");

            if(voteChoice == 1){

                voteUp(
                    currentUserId,
                    q.answers.data[answerChoice-1]
                );
            }
            else if(voteChoice == 2){

                voteDown(
                    currentUserId,
                    q.answers.data[answerChoice-1]
                );
            }

            break;
        }

        case '0':
            break;

        default:
            cout << "Invalid action.\n";
    }
}

  void sortQuestionByVote(string mode = "ASC"){
    int i, j, size = questions.length;
    
    for(i = 1; i < size; i++){
      Question temp = questions.at(i);
      int j = i-1;
      if(mode == "ASC"){
        while(j >= 0 && temp.vote < questions.data[j].vote){
            questions.data[j + 1] = questions.data[j];
            j--;
        }
      }
      else if(mode == "DESC"){
        while(j >= 0 && temp.vote > questions.data[j].vote){
            questions.data[j + 1] = questions.data[j];
            j--;
        }
      }
      else{cout << "Invalid Sorting Mode." << endl; return;}
      questions.data[j + 1] = temp;  
    }
  }

  unordered_map<string, int> sentenceToWord(string s){
    unordered_map<string, int> res;
    string word = "";
    for(char c : s){
      c = tolower(c);
      if(isspace(c)){
        res[word]++;
        word = "";
      }
      else{
        word += c;
      }
    }  
    if(word != ""){
      res[word]++;
    }   
    return res;
  }

  Array<Question> searchTitleQuestion(string target){
    struct SearchRes{
        Question questions;
        int extraWords, sameWords;
    };
    Array<SearchRes> searchRes;
    unordered_map<string, int> targetWords = sentenceToWord(target);
      for(int i = 0; i < questions.length; i++){
        unordered_map<string, int> titleWords = sentenceToWord(questions.data[i].title);
        SearchRes temp;
        temp.extraWords = 0;
        temp.sameWords = 0;
        for(auto word1 : titleWords){
          auto word2 = targetWords.find(word1.first);
          if(word2 != targetWords.end()){
            int a = word1.second, b = word2->second;
            temp.extraWords += abs(b - a);
            temp.sameWords += min(a, b);
          }
          else{
            temp.extraWords += word1.second;
          }
        }
        if(temp.sameWords > 0){
          for(auto word : targetWords){
            if(titleWords.find(word.first) == titleWords.end()){
              temp.extraWords += word.second;
            }
          }
          temp.questions = questions.data[i];
          searchRes.insert(temp);
        }
      }

    int size = searchRes.length;
    for(int i = 1; i < size; i++){
        SearchRes temp = searchRes.data[i];
        int j = i-1;
        while(j >= 0 && (temp.sameWords > searchRes.data[j].sameWords || 
          (temp.sameWords == searchRes.data[j].sameWords && temp.extraWords < searchRes.data[j].extraWords)))
        {
            searchRes.data[j + 1] = searchRes.data[j];
            j--;
        }
        searchRes.data[j + 1] = temp;  
    }

    Array<Question> finalRes;
    for(int i = 0; i < size; i++){
      finalRes.insert(searchRes.data[i].questions);
    }

    return finalRes;
}

  void userInit(string path) {
    CSV csv(path);

    csv.parse();

    Array<string> names = csv.get("name");
    Array<string> passwords = csv.get("password");
    Array<string> emails = csv.get("email");
    Array<string> reputations = csv.get("reputation");

    int length = csv.length("yang penting bukan all");

    for (int i = 0; i < length; i++) {
      createUser(names.at(i), passwords.at(i), emails.at(i));
      users.data[i].reputation = stoi(reputations.at(i));
    }
  }

private:
  void questionInit(string path) {
    CSV csv(path);

    csv.parse();

    Array<string> userIds = csv.get("authorId");
    Array<string> titles = csv.get("title");
    Array<string> bodies = csv.get("body");
    Array<string> votes = csv.get("vote");
    Array<string> isVerifieds = csv.get("isVerified");

    int length = csv.length("yang penting bukan all");

    for (int i = 0; i < length; i++) {
      createQuestion(stoi(userIds.at(i)), titles.at(i), bodies.at(i), true);
      questions.data[i].vote = stoi(votes.at(i));
      questions.data[i].isVerified = stoi(isVerifieds.at(i));
    }
  }

  void answerInit(string path) {
    CSV csv(path);
    csv.parse();

    Array<string> authorIds = csv.get("authorId");
    Array<string> texts = csv.get("text");
    Array<string> votes = csv.get("vote");
    Array<string> isAccepteds = csv.get("isAccepted");

    int length = csv.length("yang penting bukan all");

    for (int i = 0; i < length; i++) {
      const bool isAccepted = stoi(isAccepteds.at(i));
      Answer answer = {i + 1, stoi(authorIds.at(i)), texts.at(i),
                       stoi(votes.at(i)), isAccepted};
      questions.data[i].answers.insert(answer);
    }
  }

  void commentInit(string path) {
    CSV csv(path);
    csv.parse();

    Array<string> authorIds = csv.get("authorId");
    Array<string> texts = csv.get("text");
    Array<string> votes = csv.get("vote");

    int length = csv.length("yang penting bukan all");

    for (int i = 0; i < length; i++) {
      questions.data[i].addComment(stoi(authorIds.at(i)), texts.at(i));
      questions.data[i].comments.data[i].vote = stoi(votes.at(i));
    }
  }

public:
  void init() {
    userInit("./data/users.csv");
    questionInit("./data/questions.csv");
    answerInit("./data/answers.csv");
    commentInit("./data/comments.csv");
  }
};

void clearScreen() {
  #ifdef _WIN32
      system("cls");
  #else
      system("clear");
  #endif
  }

int main() {
  App app;
  app.init();
  bool isLogin = false;
  int currentUserId = -1;
  

  char choice;
  bool isMenu = true;

  cout << "Welcome to Stack Overflow!\n";

  while (isMenu) {

    cout << "\n==== MENU ====\n";
    cout << "1. Show profile\n";
    cout << "2. Show newest questions\n";
    cout << "3. Search questions\n";
    cout << "4. Ask question\n";
    cout << "0. Exit\n";
    cout << "Choose: ";
    cin >> choice;

    switch (choice) {
    case '1': {

      if (!isLogin) {

      char profileChoice;

      cout << "\n=== ACCOUNT MENU ===\n";
      cout << "1. Register\n";
      cout << "2. Login\n";
      cout << "0. Back\n";
      cout << "Choose: ";
      cin >> profileChoice;

      switch (profileChoice) {

        case '1': {
            string name = input<string>("Input username : ");
            string password = input<string>("Input password : ");
            string email = input<string>("Input email    : ");

            app.createUser(name, password, email);

            cout << "Register success.\n";

            break;
          }

        case '2': {

          string email = input<string>("Input email    : ");
          string password = input<string>("Input password : ");

          bool found = false;

          for (int i = 0; i < app.users.length; i++) {
            if (app.users.data[i].email == email &&
              app.users.data[i].password == password) {

              isLogin = true;
              currentUserId = app.users.data[i].id;

              found = true;

              cout << "Login success.\n";
              break;
            }
          }

          if (!found) {
            cout << "Login failed.\n";
          }
          break;
        }

        case '0':
          break;

        default:
          cout << "Invalid input.\n";
        }

      } 
      else {
        app.assignReputation(currentUserId);
        app.showProfile(currentUserId);
      }
      break;
    }
    case '2': {

       app.showNewestQuestions();

    int questionChoice =
        input<int>("Select Question: ");

    int realIndex =
        app.questions.length - questionChoice;

    if(realIndex < 0 ||
       realIndex >= app.questions.length)
    {
        cout << "Invalid Question.\n";
        break;
    }

    bool isDetail = true;

    while(isDetail){

        clearScreen();

        app.showQuestionDetail(realIndex);

        cout << "\n1. View Comments\n";
        cout << "2. View Answers\n";
        cout << "3. Actions\n";
        cout << "0. Back\n";

        char detailChoice;

        cout << "Choose: ";
        cin >> detailChoice;

        switch(detailChoice){

            case '1':

                  clearScreen();

                app.showComment(realIndex + 1);

                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();

                break;
2
            case '2':

               clearScreen();

                app.showAnswerList(realIndex + 1);

                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();

                break;

            case '3':

                if(!isLogin){

                    clearScreen();

                      cout << "====================\n";
                      cout << " Please login first \n";
                      cout << "====================\n";

                      cout << "\nPress Enter...";
                      cin.ignore();
                      cin.get();
                }
                else{

                    app.questionActions(
                        realIndex,
                        currentUserId
                    );
                }

                break;

            case '0':

                isDetail = false;

                break;

            default:

                cout << "Invalid input.\n";
        }
    }
    break;
  }
    case '3': {
      char searchMenuChoice;
      int searchChoice;
      cout << "1. ASC" << endl;
      cout << "2. DESC" << endl;
      cout << "3. Title" << endl;
      cout << "Choose: "; cin >> searchMenuChoice;

      switch(searchMenuChoice){
        case '1':{
          app.sortQuestionByVote("ASC");
          app.showAllQuestions();
          searchChoice = input<int>("Choose question: ");
          break;
        }
        case '2':{
          app.sortQuestionByVote("DESC");
          app.showAllQuestions();
          searchChoice = input<int>("Choose question: ");
          break;
        }
        case '3':{
          string target;
          Array<Question> res;
          target = input<string>("enter title: ");
          Array<Question> searchResult = app.searchTitleQuestion(target);
          for(int i = 0; i < searchResult.length; i++){
            cout << i + 1 << ". " << searchResult.data[i].title << endl;
          }
          searchChoice = input<int>("Choose question: ");
          break;
        }
        case '0':{
          break;
        }
        default:{
          break;
        }
      }
      cout << searchChoice;
      break;
    }
    case '4': {
		
		if (!isLogin) {
			cout << "Please login first.\n";
			break;
		}
		
	  string title = input<string>("Input title : ");
	  string body = input<string>("Input body  : ");

	  app.createQuestion(currentUserId, title, body);
      break;
    }
    case '0': {
      isMenu = false;
      break;
    }
    default:
      cout << "Invalid input.\n";
      break;
    }
  }
  return 0;
}

template <> string input<string>(string text) {
  string n;

  do {
    cout << text;
    getline(cin >> ws, n);
  } while (n.empty());

  return n;
}

template <typename T> T input(string text) {
  T n;

  cout << text;
  while (!(cin >> n)) {
    cout << "Invalid input. Try again: ";
    cin.clear();
    cin.ignore(10000, '\n');
  }

  return n;
}
