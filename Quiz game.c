#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_QUESTIONS 5
#define MAX_TEXT 1024
#define MAX_OPTION 64

struct Question {
    char text[MAX_TEXT];
    char options[4][MAX_OPTION];
    char correct_answer;
};

int parse_questions(const char* filename, struct Question questions[], int max_questions) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    char line[1024];
    int q_count = 0;

    while (fgets(line, sizeof(line), file) && q_count < max_questions) {
        if (line[0] >= '1' && line[0] <= '9' && line[1] == '.') {
            questions[q_count].text[0] = '\0';
            strcat(questions[q_count].text, line);

            while (fgets(line, sizeof(line), file) && strncmp(line, "a)", 2) != 0) {
                strcat(questions[q_count].text, line);
            }

            for (int i = 0; i < 4; i++) {
                sscanf(line, "%*c) %63[^\n]", questions[q_count].options[i]);
                fgets(line, sizeof(line), file);
            }

            fgets(line, sizeof(line), file);
            sscanf(line, "Answer: (%c)", &questions[q_count].correct_answer);
            q_count++;
        }
    }

    fclose(file);
    return q_count;
}

int main() {
    printf("Welcome to the C Programming Quiz Game!\n");
    printf("Choose a question set (1, 2, or 3): ");

    int choice;
    while (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid choice. Please enter 1, 2, or 3: ");
        while (getchar() != '\n');
    }
    getchar(); // Consume newline

    const char* filenames[] = {"quiz_question_1.txt", "quiz_question_2.txt", "quiz_question_3.txt"};
    struct Question questions[MAX_QUESTIONS];
    int num_questions = parse_questions(filenames[choice - 1], questions, MAX_QUESTIONS);

    if (num_questions <= 0) {
        printf("Failed to load questions. Exiting...\n");
        return 1;
    }

    int score = 0;
    char user_answer[10];

    for (int i = 0; i < num_questions; i++) {
        printf("\nQuestion %d:\n%s", i + 1, questions[i].text);
        printf("a) %s\n", questions[i].options[0]);
        printf("b) %s\n", questions[i].options[1]);
        printf("c) %s\n", questions[i].options[2]);
        printf("d) %s\n", questions[i].options[3]);
        printf("Your answer (a, b, c, d): ");

        fgets(user_answer, sizeof(user_answer), stdin);
        user_answer[strcspn(user_answer, "\n")] = 0;

        while (strlen(user_answer) != 1 || strchr("abcd", tolower(user_answer[0])) == NULL) {
            printf("Invalid input. Enter a, b, c, or d: ");
            fgets(user_answer, sizeof(user_answer), stdin);
            user_answer[strcspn(user_answer, "\n")] = 0;
        }

        char answer = tolower(user_answer[0]);
        if (answer == questions[i].correct_answer) {
            printf("Correct!\n");
            score++;
        } else {
            printf("Wrong. The correct answer is %c.\n", questions[i].correct_answer);
        }
    }

    float percentage = (float)score / num_questions * 100;
    printf("\nQuiz complete! Your score: %d out of %d (%.2f%%)\n", score, num_questions, percentage);

    return 0;
}
