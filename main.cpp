#include <iostream>

struct Ball {
    int position{};
    bool heading_right = true;
};

struct BallList {
    Ball *ball = nullptr;
    BallList *next = nullptr;
};

void turnHead(Ball *ball);

void updatePosition(Ball *ball, int n, int delta_t);

int findNextCrash(Ball *ball, BallList *crashing_list, int n, int L);

void deleteLinkedList(BallList *ballList);

void headInsert(BallList *ballList, Ball *ball);

int main() {
    int n, L, t;
    std::cout << "Please enter n:" << std::endl;
    std::cin >> n;
    std::cout << "Please enter L:" << std::endl;
    std::cin >> L;
    std::cout << "Please enter t:" << std::endl;
    std::cin >> t;

    Ball ball[n];
    auto *crashing_list = new BallList;

    for (int i = 0; i < n; ++i) {
        std::cout << "Please enter the position of ball " << i + 1 << " :" << std::endl;
        std::cin >> (ball + i)->position;
    }

    while (true) {
        int delta_t = findNextCrash(ball, crashing_list, n, L);
        std::cout << "next delta t: " << delta_t << std::endl;
        std::cout << "rest t: " << t << std::endl;

        updatePosition(ball, n, delta_t);

        while (crashing_list->next) {
//            std::cout << "!!!crash position: " << crashing_list->ball->position << std::endl;
            turnHead(crashing_list->ball);
            std::cout << "ball " << crashing_list->ball->position << " turn head !" << std::endl;
            crashing_list = crashing_list->next;
        }
        deleteLinkedList(crashing_list);
        if (t <= delta_t)
            break;
        t -= delta_t;
    }
    updatePosition(ball, n, t);
    std::cout << "Positions of all balls at the end of t :" << std::endl;
    for (int i = 0; i < n; ++i) {
        updatePosition(ball + i, n, t);
        std::cout << (ball + i)->position;
    }
    delete crashing_list;
}

void updatePosition(Ball *ball, int n, int delta_t) {
    for (int i = 0; i < n; ++i) {
        if ((ball + i)->heading_right) {
            (ball + i)->position += delta_t;
            std::cout << "move right, ball " << 1 + i << " position: " << (ball + i)->position << std::endl;
        } else {
            (ball + i)->position -= delta_t;
            std::cout << "move left, ball " << 1 + i << " position: " << (ball + i)->position << std::endl;

        }
    }
}

void turnHead(Ball *ball) { ball->heading_right = !ball->heading_right; }

int findNextCrash(Ball *ball, BallList *crashing_list, int n, int L) {
    int time_left_wall = ball->position;
    int time_right_wall = L - (ball + n - 1)->position;
    int min_crashing_time = L;

    if (!ball->heading_right && time_left_wall <= min_crashing_time) {
        min_crashing_time = time_left_wall;
        headInsert(crashing_list, ball);
    }

    if ((ball + n - 1)->heading_right && time_right_wall <= min_crashing_time) {
        if (time_right_wall < min_crashing_time)
            deleteLinkedList(crashing_list);
        min_crashing_time = time_right_wall;
        headInsert(crashing_list, ball + n - 1);
    }

    if ((ball + n - 2)->heading_right && !(ball + n - 1)->heading_right) {
        if (min_crashing_time >= ((ball + n - 2)->position - (ball + n - 2)->position) / 2) {
            if (min_crashing_time > ((ball + n - 2)->position - (ball + n - 2)->position) / 2)
                deleteLinkedList(crashing_list);
            min_crashing_time = ((ball + n - 2)->position - (ball + n - 2)->position) / 2;
            headInsert(crashing_list, ball + n - 1);
            headInsert(crashing_list, ball + n - 2);
        }
    }

    for (int i = 0; i < n - 1; ++i) {
        if ((ball + i)->heading_right && !(ball + i + 1)->heading_right) {
            if (min_crashing_time >= ((ball + i + 1)->position - (ball + i)->position) / 2) {
                if (min_crashing_time > ((ball + i + 1)->position - (ball + i)->position) / 2)
                    deleteLinkedList(crashing_list);
                min_crashing_time = ((ball + i + 1)->position - (ball + i)->position) / 2;
                headInsert(crashing_list, ball + i);
                headInsert(crashing_list, ball + i + 1);
            }
        }
    }
    return min_crashing_time;
}

void deleteLinkedList(BallList *ballList) {
    if (ballList->next == nullptr)
        return;
    ballList->next->ball = nullptr;
    BallList *ptr1 = ballList->next->next;
    BallList *ptr2;
    while (ptr1) {
        ptr2 = ptr1->next;
        delete ptr1;
        ptr1 = ptr2;
    }
}

void headInsert(BallList *ballList, Ball *ball) {
    auto *node = new BallList;
    node->ball = ball;
    node->next = ballList->next;
    ballList->next = node;
}

