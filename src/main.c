#include <graphics_lib.h>      //Y3916929
#include <amio.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#define RADIUS 10
#define XWINDOW 640
#define YWINDOW 480
#define XMAX    XWINDOW - RADIUS
#define XMIN    RADIUS
#define YMAX    YWINDOW - RADIUS
#define YMIN    RADIUS
#define BALL_INITIAL_X_POSITION 120
#define BALL_INITIAL_Y_POSITION 340
#define NUMBER_COLORS 16

#define GRAVITY 9.8
//defining a structure for ball
typedef struct
ki{
    double x_new, y_new;
    double vx, vy;
    uint8_t color;
} Ball;

//defining a structure for person
typedef struct //
{
    double    head_x;
    double    head_y;
    double    head_radius;
} Person;


/* Function which checks if the ball has hit the boundary. */
bool ball_hit_boundary(Ball * ball){
    if (ball->x_new > XMAX || ball->x_new < XMIN || ball->y_new > YMAX || ball->y_new < YMIN){
        return true;
    } else {
        return false;
    }
}

/* Function to draw a basic stick person based on the position of the thrower and catcher. */
void draw_stick_person(Person * person, bool thrower){
    // drawing head
    circle(person->head_x, person->head_y, person->head_radius, 3);
    // drawing body
    line(person->head_x, person->head_y + person->head_radius, person->head_x, person->head_y + 50, 3);

    // drawing legs
    line(person->head_x, person->head_y + 50, person->head_x - 20, person->head_y + 90, 3);
    line(person->head_x, person->head_y + 50, person->head_x + 20, person->head_y + 90, 3);

    // drawing hands
    if(thrower){
        line(person->  head_x, person->head_y + 30, person->head_x - 20, person->head_y + 40, 3);
        line(person->head_x - 20, person->head_y + 40, person->head_x - 30, person->head_y + 55, 3);
        line(person->head_x, person->head_y + 30, person->head_x + 15, person->head_y + 20, 3);
        line(person->head_x + 15, person->head_y + 20, person->head_x + 20, person->head_y + 10, 3);
    } else {
        line(person->head_x, person->head_y + 30, person->head_x + 20, person->head_y + 40, 3);
        line(person->head_x + 20, person->head_y + 40, person->head_x + 30, person->head_y + 55, 3);
        line(person->head_x, person->head_y + 30, person->head_x - 15, person->head_y + 20, 3);
        line(person->head_x - 15, person->head_y + 20, person->head_x - 20, person->head_y + 10, 3);
    }
}

/* Updates the position of the ball based on the projectile formula. */
void update_ball_position(Ball * ball){
    double time = (ball->x_new - 120)/ ball->vx;
    ball->y_new = (340 - (ball->vy * time) + (GRAVITY * time * time)/2);
    ball->x_new = ball->x_new + 1;
}
/*sets the values of the ball as random  */
void set_random_ball_values(Ball * ball) {
    ball->vx = (double) (rand_number(20, 40));
    ball->vy = (double) (rand_number(40, 60));
    ball->color = (uint8_t) rand_number(1, NUMBER_COLORS-1);
}
/* The main function*/
int main(void)
{
    bool end_trial_game = false;
    bool quit_game = false;
    bool difficulty_options_open = false;
    bool difficult_game = false;
    int lives_lost = 0;
    int number_of_times_caught = 0;
    /*initialise amio audio*/
    amio_init_audio();

    /*Load samples*/
    amio_load_sample("main", "./samples/drums.wav");
    amio_load_sample("spectra", "./samples/spectra16.wav");

    /*Background music sample*/
    amio_add_sample_instance("main", PLAY_LOOP, 1.0);
    Person thrower;
    Person catcher;

    /*Create array of balls*/
    Ball ball;

    ball.x_new = (int) BALL_INITIAL_X_POSITION;
    ball.y_new = (int) BALL_INITIAL_Y_POSITION;

    thrower.head_x = 100;
    thrower.head_y = 330;
    thrower.head_radius = 10;

    catcher.head_x = 550;
    catcher.head_y = 330;
    catcher.head_radius = 10;

    /* open the graphics window */
    initwindow(XWINDOW, YWINDOW);

    /* Load bitmap after initialising window */
    BITMAP bkg_img = load_bitmap("./data/spacebackground.png");

    void initialise_ball_position(){
        ball.x_new = BALL_INITIAL_X_POSITION;
        ball.y_new = BALL_INITIAL_Y_POSITION;
    }

    /* Function to check if any arrow key is pressed and change the position of the catcher based on the event. */
    void add_mouse_events(){
        if (check_if_event()){
            /* wait for event  */
            wait_for_event();
            if(event_key_left_arrow()){
                if(catcher.head_x - 15 < XWINDOW && catcher.head_x - 15 > 0){
                    catcher.head_x = catcher.head_x - 5;
                }
            }
            else if(event_key_right_arrow()){
                if(catcher.head_x + 25 < XWINDOW && catcher.head_x + 25 > 0){
                    catcher.head_x = catcher.head_x + 5;
                }
            }
        }
    }

    /* Function to display the score based on the number of catches made. */
    void display_score() {
        char catches[32];
        sprintf(catches, "%d", number_of_times_caught);
        outtextxy(25, 25, "Score:");
        outtextxy(80, 25, catches);
    }

    /*Function to display lives left for the catcher. */
    void display_lives(){
        char lives[12];
        sprintf(lives, "%d", 3-lives_lost);
        rectangle(480, 15, 620, 50, 2);
        // draw life 1
        if(lives_lost <= 2){
            line(500, 25, 520, 40, 2);
            line(500, 40, 520, 25, 2);
        }

        // draw life 2
        if(lives_lost <= 1){
            line(535, 25, 555, 40, 2);
            line(535, 40, 555, 25, 2);
        }

        // draw life 3
        if(lives_lost <= 0){
            line(570, 25, 590, 40, 2);
            line(570, 40, 590, 25, 2);
        }
        outtextxy(600, 25, lives);
    }

    /* Function to check if the ball has been caught. A circle with a radius of 15 units around the hand of
    the catcher is */
    bool check_if_ball_is_caught(){
        int catcher_hand_position_x = catcher.head_x - 20;
        int catcher_hand_position_y = catcher.head_y + 10;
        double term_one = (ball.x_new - catcher_hand_position_x) * (ball.x_new - catcher_hand_position_x);
        double term_two = (ball.y_new - catcher_hand_position_y) * (ball.y_new - catcher_hand_position_y);
        if(term_one +  term_two < 225){
            return true;
        } else {
            return false;
        }
    }

    bool should_quit(){
        if(check_if_event()){
        /* if there is a keyboard event */
            if (event_key_down()){
                printf("event");
                if  (event_key('q')){
                    printf("event q");
                    quit_game = true;
                    return true;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    void draw_ball(){ //function to draw ball
        draw_bitmap(bkg_img, (int)XWINDOW/2, (int)YWINDOW/2);
        if(difficult_game){
            if((ball.x_new >= BALL_INITIAL_X_POSITION && ball.x_new <= 300) || (ball.x_new >= 380 && ball.x_new <= XWINDOW)){
                filled_circle(ball.x_new, ball.y_new, RADIUS , ball.color);
            }
        } else {
            filled_circle(ball.x_new, ball.y_new, RADIUS , ball.color);
        }
    }

    void display_instructions(){ //function to display instructions
        rectangle(100, 60, 530, 180, 2);
        outtextxy(120, 80,"This is a simple game of catch.");
        outtextxy(120, 100, "Use the right and left arrows in your keyboard");
        outtextxy(120, 120, "to move around. Make sure the ball is in the");
        outtextxy(120, 140, "hand. Have fun!");
    }

    void draw_thrower_catcher_on_screen(bool display_instruction){ //function to call thrower stickman
        draw_stick_person(&thrower, true);
        draw_stick_person(&catcher, false);
        if(display_instruction){
            display_instructions();
        }
    }

    void throw_ball(){ //function to throw ball
        bool ball_is_caught = false;
        do {
            add_mouse_events();
            draw_ball();
            display_lives();
            display_score();
            draw_thrower_catcher_on_screen(false);
            ball_is_caught = check_if_ball_is_caught();
            if(ball_is_caught){
                // Initialize position of ball to its initial value and break out of the loop.
                number_of_times_caught++;
                initialise_ball_position();
                break;
            }
            update_ball_position(&ball);
            update_display();
            pausefor(8);
        }
        while(ball_hit_boundary(&ball) == false);
    }

    void display_game_over_message(){ //function to display message of game over in real game
        difficulty_options_open = false;
        draw_bitmap(bkg_img, (int)XWINDOW/2, (int)YWINDOW/2);
        draw_thrower_catcher_on_screen(false);
        rectangle(100, 100, 530, 200, 2);
        outtextxy(160,150,"Game over!!! Click here to begin again.");
    }

    void display_trial_game_over_message(){ //function to display message of game over in trial
        draw_bitmap(bkg_img, (int)XWINDOW/2, (int)YWINDOW/2);
        draw_thrower_catcher_on_screen(false);
        rectangle(100, 100, 530, 160, 2);
        outtextxy(270,110,"Good job!!!");
        outtextxy(210,130,"Click here to begin the game.");
    }

    void begin_game(){ //function for beginning of the game
        lives_lost = 0;
        number_of_times_caught = 0;
        do {
            set_random_ball_values(&ball);
            throw_ball();
            if(ball_hit_boundary(&ball) == true){
                initialise_ball_position();
                ++lives_lost;
            }
            if(lives_lost == 3){
               display_game_over_message();
            }
        }
        while(lives_lost <= 2);
    }

    void throw_trial_ball(){ //function for trial ball
        do {
            add_mouse_events();
            draw_ball();
            draw_thrower_catcher_on_screen(true);
            if(check_if_ball_is_caught()){
                end_trial_game = true;
                display_trial_game_over_message();
                number_of_times_caught = 0;
                break;
            }
            update_ball_position(&ball);
            update_display();
            pausefor(8);
        }
        while(ball_hit_boundary(&ball) == false);
    }


    void begin_trial_game(){
        do {
            set_random_ball_values(&ball);
            throw_trial_ball();
            if(ball_hit_boundary(&ball) == true){
                initialise_ball_position();
            }
        }
        while(end_trial_game == false);
    }

    void show_difficulty_options(){
        difficulty_options_open = true;
        cleardevice(); // To clear previous display.
        outtextxy(230,50,"Choose difficulty level!");
        outtextxy(300,150,"Easy");
        outtextxy(280,350,"Difficult");
        rectangle(100, 100, 530, 200, 2);
        rectangle(100, 300, 530, 400, 2);
    }

    /* allow mouse operations */
	initmouse();
	initkeyboard();

    /* create an event queue */
    create_event_queue();

    /* register display and mouse as event sources */
    reg_display_events();
    reg_mouse_events();
    reg_keyboard_events();

    /* initialize the font */
    initfont();

    update_display();
    outtextxy(235,150,"Click here to begin!");
    outtextxy(260,350,"Trial game");
    rectangle(100, 100, 530, 200, 2);
    rectangle(100, 300, 530, 400, 2);

	do
	{
        update_display();
        /* update amio audio */
        amio_update_audio();

        /* check for user events */

        if (check_if_event())
        {
            /* wait for event  */
            wait_for_event();
            /* if the window is closed */
            if (event_close_display()){
                break;
            }
            if (event_mouse_button_down()){
                if (event_mouse_left_button_down()){
                    if(event.mouse.x > 100 && event.mouse.x < 530 && event.mouse.y > 100 && event.mouse.y < 200){
                        if(difficulty_options_open == false){
                            show_difficulty_options();
                        } else {
                            difficult_game = false;
                            begin_game();
                        }
                    }
                    else if(event.mouse.x > 100 && event.mouse.x < 530 && event.mouse.y > 300 && event.mouse.y < 400){
                        if(difficulty_options_open == false){
                            begin_trial_game();
                        } else {
                            difficult_game = true;
                            begin_game();
                        }
                    }
                }
            }
        }
	}
    while (quit_game == false);


    /* destroy audio */
    amio_destroy_audio();

    /* close the mouse */
    closemouse();

    /* close the keyboard */
    closekeyboard();

    /* remove the display */
    closegraph();


    return 0;
}

