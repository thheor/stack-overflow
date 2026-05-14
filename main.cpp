#include <iostream>
#include <string>
using namespace std;

template <typename T> T input(string text);

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
  int view = 0;
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
  int votes = 0;
};

struct App {
  Array<User> users;
  Array<Question> questions;

  template <typename T> void assignData(T data) {}

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
      cout << i + 1 << ". " << questions.data[i].title << "\n";
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

    cout << "\n";
    cout << "Username\t: " << user.name << "\n";
    cout << "Email\t\t: " << user.email << "\n";
    cout << "Reputation\t: " << user.reputation << "\n";
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
      questions.data[i].addAnswer(stoi(authorIds.at(i)), texts.at(i));
      questions.data[i].answers.data[i].vote = stoi(votes.at(i));
      questions.data[i].answers.data[i].isAccepted = stoi(votes.at(i));
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

int main() {
  User user; // cuma data dummy, nanti diilangin biar ada Guest mode
  App app;
  app.init();

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
      user.id = 1;
      app.showProfile(user.id);
      user.id = 2;
      app.showProfile(user.id);
      user.id = 3;
      app.showProfile(user.id);
      user.id = 4;
      app.showProfile(user.id);
      user.id = 5;
      app.showProfile(user.id);
      user.id = 6;
      app.showProfile(user.id);
      break;
    }
    case '2': {
      app.showAllQuestions();
      break;
    }
    case '3': {
      break;
    }
    case '4': {
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

template <typename T> T input(string text) {
  T n;

  if (typeid(n) == typeid(string)) {
    do {
      cout << text;
      getline(cin, n);
    } while (n.empty());

    return n;
  }

  cout << text;
  while (!(cin >> n)) {
    cout << "Invalid input. Try again: ";
    cin.clear();
    cin.ignore(10000, '\n');
  }

  return n;
}
