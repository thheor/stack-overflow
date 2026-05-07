#include <iostream>
using namespace std;

template <typename T> struct Array {
  int length;
  int capacity;
  T *data;

  Array(int capacity = 2)
      : length(0), capacity(capacity), data(new T[capacity]) {}

  void insert(T &value) {
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

  void print() {
    for (int i = 0; i < length; i++) {
      cout << data[i] << "\n";
    }
  }
};

struct Badges {
  string curious = "🐱";
  string guru = "🎎";
  string greatAnswer = "🏅";
  string supporter = "🫴";
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
  int view = 0;
  int vote = 0;
  bool isVerified = false;
  Array<string> tags;
  Array<Answer> answers;
  Array<Comment> comments;

  void addAnswer(string text, int userId) {
    Answer input;
    int id = answers.length + 1;
    input = {id, userId, text};
    answers.insert(input);

    cout << "Answer added successfully.";
  }

  void addComment(string text, int userId) {
    Comment input;
    int id = comments.length + 1;
    input = {id, userId, text};
    comments.insert(input);

    cout << "Comment created successfully.";
  }
};

struct User {
  int id = 1;
  string name = "Mangaras";
  string password = "mangaras";
  string email = "mangaras@gmail.com";
  int reputation = 123;
  int votes = 0;
  Array<string> badges;
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

  void createQuestion(int userId) {
    int authorId = userId;

    Question input;
    input.id = questions.length;
    input.authorId = authorId;
    Array<string> tags(5);

    cout << "Your question:\n";
    cout << "Title: ";
    cin.ignore();
    getline(cin, input.title);
    cout << "Body: ";
    getline(cin, input.body);

    int n;
    cout << "How many tags do you want to add? \n";
    cin >> n;
    cout << "Add " << n << " tags: \n";
    string tag[n];
    for (int i = 0; i < n; i++) {
      cout << "Tag #" << i + 1 << ": ";
      cin >> tag[i];
      tags.insert(tag[i]);
    }
    input.tags = tags;

    questions.insert(input);

    cout << "Question created successfully.\n";
  }

  void showAllQuestions() {

    if (questions.length == 0) {
      cout << "\nNo questions in database.\n";
      return;
    }
    cout << "All Questions\n";
    for (int i = 0; i < questions.length; i++) {
      cout << i + 1 << ". " << questions.data[i].title << "\n";
    }
  }

  template <typename T> void showQuestions(T var) {}

  template <typename T> void voteUp(T &object, int userId) {
    object.vote += 1;

    cout << "Vote up successfully.";
  }

  template <typename T> void voteDown(T &object, int userId) {
    object.vote -= 1;

    cout << "Vote down successfully.";
  }

  void assignBadges(int userId) {
    User user = users.data[userId - 1];
    int id = user.id;
    int totalQuestions = 0;
    int totalAnswers = 0;
    int totalAnswersVoted = 0;
    int totalBadges = user.badges.length;

    for (int i = 0; i < questions.length; i++) {
      if (questions.data[i].authorId == id) {
        totalQuestions++;
      }
    }

    for (int i = 0; i < questions.length; i++) {
      for (int j = 0; j < questions.data[i].answers.length; j++) {
        if (questions.data[i].answers.data[j].authorId == id) {
          totalQuestions++;
        }
      }
    }

    for (int i = 0; i < questions.length; i++) {
      for (int j = 0; j < questions.data[i].answers.length; j++) {
        if (questions.data[i].answers.data[j].authorId == id) {
          totalAnswersVoted++;
        }
      }
    }

    Badges badges;

    if (totalBadges > 4)
      return;

    if (totalQuestions >= 5) {
      user.badges.data[user.badges.length++] = badges.curious;
    }

    if (totalAnswers >= 5) {
      user.badges.data[user.badges.length++] = badges.guru;
    }

    if (totalAnswersVoted >= 10) {
      user.badges.data[user.badges.length++] = badges.greatAnswer;
    }

    if (user.votes >= 10) {
      user.badges.data[user.badges.length++] = badges.supporter;
    }
  }

  void assignReputation(int userId) {
    User user = users.data[userId - 1];
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
    cout << "Badges\t\t: ";

    int size = user.badges.length;
    if (size == 0) {
      cout << 0;
    } else {
      for (int i = 0; i < size; i++) {
        cout << user.badges.data[i] << ", ";
      }
    }

    cout << "\n";
  }
};

int main() {
  User user; // cuma data dummy, nanti diilangin biar ada Guest mode
  App app;

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
      app.createQuestion(user.id);
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
