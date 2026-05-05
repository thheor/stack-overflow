#include <iostream>
using namespace std;

int indexQ = 0;
int indexA = 0;
int indexC = 0;
const int constraint = 100;

enum QorA { QUESTION, ANSWER };

struct Badges {
  string curious = "🐱";
  string guru = "🎎";
  string greatAnswer = "🏅";
  string supporter = "🫴";
};

struct Question {
  string title;
  string body;
  int view;
  int vote;
  string tags[5];
  bool status;
};

struct Answer {
  string text;
  int view;
  int vote;
  bool isAccepted;
};

struct Comment {
  string text;
  int vote;
  QorA status;
};

struct Tag {
  string name;
  string description;
};

struct User {
  string name = "test";
  string password;
  string email;
  Question questions[constraint];
  Answer answers[constraint];
  Comment comments[constraint];
  double reputation;

  void createQuestion() {

    cout << "Your question:\n";
    cout << "Title: ";
    cin.ignore();
    getline(cin, questions[indexQ].title);
    cout << "Body: ";
    getline(cin, questions[indexQ].body);

    int n;
    cout << "How many tags do you want to add? \n";
    cin >> n;
    cout << "Add " << n << " tags (separated by space): ";
    for (int i = 0; i < n; i++) {
      cin >> questions[indexQ].tags[i];
    }

    cout << "Question created successfully.";
    indexQ++;
  }

  void createAnswer() {
    cout << "Your answer:\n";
    cin >> answers[indexA].text;

    cout << "Answer created successfully.";
    indexA++;
  }

  void createComment() {
    cout << "Your comment:\n";
    cin >> comments[indexC].text;

    cout << "Comment created successfully.";
    indexC++;
  }

  void voteUpQuestion(Question question) {
    question.vote += 1;

    cout << "Vote up question successfully.";
  }

  void voteDownQuestion(Question question) {
    question.vote -= 1;

    cout << "Vote down question successfully.";
  }
};

struct System {
  void assignBadges() {}

  void assignReputation() {}
};

int main() {
  User totalUser[constraint];
  User user = totalUser[0];

  char choice;
  bool isMenu = true;

  while (isMenu) {

    cout << "Welcome to Stack Overflow!\n\n";
    cout << "==== MENU ====\n";
    cout << "1. Show profile\n";
    cout << "2. Show newest questions\n";
    cout << "3. Search questions\n";
    cout << "4. Ask question\n";
    cout << "0. Exit\n";
    cout << "Choose: ";
    cin >> choice;

    switch (choice) {
    case '1': {
      break;
    }
    case '2': {
      break;
    }
    case '3': {
      break;
    }
    case '4': {
      user.createQuestion();
      cout << endl;
      cout << "BREAKDOWN\n\n";
      cout << user.questions[0].title << endl;
      cout << user.questions[0].body << endl;
      for (auto tag : user.questions[0].tags)
        cout << tag << " ";
      break;
    }
    case '0': {
      isMenu = false;
      break;
    }
    default:
      cout << "Invalid input.";
      break;
    }
  }
  return 0;
}
