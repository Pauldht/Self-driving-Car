#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "Display.h"
#include "Circuit.h"
#include "Simulation.h"

Simulation* NewSimulation()
{
    Simulation* sim = malloc(sizeof(Simulation));

    sim->disp = newDisplay();
    sim->track = initCircuit("tracks/circle.track");
    sim->img = LoadImg("icon/carf1.bmp");
    sim->P = NewPopulation();
    return sim;
}

void FreeSimulation(Simulation* sim)
{
    freeDisplay(sim->disp);
    freeCircuit(sim->track);    
    SDL_FreeSurface(sim->img);
    FreePopulation(sim->P);
    free(sim);
}

void Display_Info(Simulation* sim, int nb_g, int fitness)
{
    if(TTF_Init() == -1)
    {
	    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
    }

    TTF_Font* police = NULL;
    police = TTF_OpenFont("fonts/Roboto.ttf", 14); 

    char sentence[100];
    sprintf(sentence, "Generation : %d  \n Fitness Max : %d", nb_g,fitness);

    SDL_Color white = {255, 255, 255,255};
    SDL_Surface* text = NULL;

    text = TTF_RenderText_Blended(police,sentence,white);

    SDL_Texture* texture = NULL;
    texture = SDL_CreateTextureFromSurface(sim->disp->renderer,text);
    assert (texture!=NULL);

    SDL_Rect rect = {230,220,text->w,text->h};
    SDL_RenderCopy(sim->disp->renderer,texture,NULL,&rect);
    
    TTF_CloseFont(police);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(text);

    TTF_Quit();
}

void delay(int * lastTime, int frameRate)
{	
	int actualTime = SDL_GetTicks();
	if(actualTime - *lastTime > 1000/frameRate)
	{
		*lastTime = actualTime;
	}
	else 
	{
		SDL_Delay(1000/frameRate - (actualTime - *lastTime));	
		delay(lastTime,frameRate);
	}
}

void Start_process()
{

    int reset = 0;
    


    do{

        srand(time(NULL));
        Simulation* sim = NewSimulation();
        InitPopulation(sim->P,sim->img);
        int ticks = SDL_GetTicks();
        SDL_Surface* img = NULL;
        SDL_Surface* image = LoadImg("icon/carf1.bmp");
        int count,brk;
        int rot = 0;//rotation factor increase left decrease right
        int accelerate = 1;
        int fitness_max = 0;
        int see_best = 1;
        int new_track = 0;
        Network* BestCar = NewNetwork();
        InitNetwork(BestCar,image);
        SDL_FreeSurface(image);

        SDL_Event event;
        int run = 1;

        int y = 1;


    while(run)
    {
        reset++;

        float activation = (rand()/(float)RAND_MAX)/5 + 0.2;
        if(activation > 0.8)
            activation = 0.8;
        if(activation < 0.2)
            activation = 0.2;

        y = 1;

        for(int i = 0; i<30; i++)
        {
            if(y == 1){

            img = sim->img;
            count = 0;
            brk = 0;
            rot = 0;

            while(DetectColision(sim->P->Generation[i].car,sim->track)==0 && brk == 0)//intersection voiture-circuit && score<22 
            {
                while(SDL_PollEvent(&event))
                {
                    switch(event.type)
                    {
                            case SDL_QUIT:
                                run = 0;
                                brk = 1;
                                y = 0;
                                break;
                            case SDL_MOUSEBUTTONDOWN:
                                if(event.button.button == SDL_BUTTON_LEFT)
                                {
                                    if(event.button.x >= 645 && event.button.x <= 790 && event.button.y >= 45 && event.button.y <= 85)
                                    {
                                        accelerate++;
                                        break;
                                    }
                                    else if(event.button.x >= 645 && event.button.x <= 790 && event.button.y >= 230 && event.button.y <= 266)
                                    {
                                        reset = 0; //créer variable reset 
                                        run = 0;
                                        brk = 1;
                                        y = 0;
                                        break;
                                    }
                                    else if(event.button.x >= 645 && event.button.x <= 790 && event.button.y >= 138 && event.button.y <= 177)
                                    {
                                        new_track +=1;
                                        freeCircuit(sim->track);

                                        switch(new_track%2)
                                        {
                                            case 0:
                                                sim->track = initCircuit("tracks/circle.track");
                                                break;
                                            case 1:
                                                sim->track = initCircuit("tracks/basic.track");
                                                break;
                                            default:
                                                break;
                                        }

                                        fitness_max = 0;
                                    }
                                    else if(event.button.x >= 645 && event.button.x <= 790 && event.button.y >= 319 && event.button.y <= 358)
                                    {
                                        see_best +=1;
                                    }
                                    else if(event.button.x >= 645 && event.button.x <= 790 && event.button.y >= 414 && event.button.y <= 453)
                                    {
                                        run = 0;
                                        brk = 1;
                                        y = 0;
                                    }
                                }
                                break;
                            default:
                                break;
                    }    
                }

                if(see_best%2 == 0)
                {
                    if(DetectColision(BestCar->car,sim->track)==0)
                    {
                        see_best+=1;
                    }

                    Input(BestCar->car,sim->track,BestCar->input);
                    //Feedforward création de l'output
                    FeedForward(BestCar);
                    //analyse de l'output
                    double max = 0;
                    int index = -1;
                    for(int y = 0; y<2; y++)
                    {
                        if(BestCar->output[y] > max)
                        {
                            max = BestCar->output[y];
                            index = y;
                        }
                    }

                    if(max > activation){
                        switch(index)
                        {
                            case 0:
                                Accelerate(BestCar->car);
                                break;

                            case 1:
                                Brake(BestCar->car);
                                break;

                            default :
                                break;
                        }
                    }

                    max = 0;

                    for(int y = 2; y<4; y++)
                    {
                        if(BestCar->output[y] > max)
                        {
                            max = BestCar->output[y];
                            index = y;
                        }
                    }

                    if(max > activation){
                        switch(index)
                        {
                            case 2: 
                                if(BestCar->car->speed > 0.5)
                                {TurnLeft(BestCar->car);
                                rot+=1;}
                                break;
                            case 3: 
                                if(BestCar->car->speed > 0.5)
                                {TurnRight(BestCar->car);
                                rot-=1;}
                                break;
                            default:
                                break;
                        }
                    }

                    if(rot > 128 || rot < -256)
                        brk = 1;

                    //update l'affichage 
                    if(accelerate%2 == 1)
                    {
                        delay(&ticks,60);
                    }
                    Draw_Background(sim->disp->renderer);
                    Display_Info(sim,sim->P->nb_of_population,fitness_max);
                    Display_Button(sim->disp->renderer,1);
                    MoveForward(BestCar->car);
                    UpdateCar(BestCar->car);
                    Rotation(sim->disp->renderer,BestCar->car,img,rot*1.40625);
                    Draw_Circuit(sim->disp->renderer,sim->track);
                    ScoreUpdate(BestCar->car,sim->track);

                }
                else{
                    //créer input à partir des sensors
                    Input(sim->P->Generation[i].car,sim->track,sim->P->Generation[i].input);
                    //Feedforward création de l'output
                    FeedForward(&sim->P->Generation[i]);
                    //analyse de l'output
                    double max = 0;
                    int index = -1;
                    for(int y = 0; y<2; y++)
                    {
                        if(sim->P->Generation[i].output[y] > max)
                        {
                            max = sim->P->Generation[i].output[y];
                            index = y;
                        }
                    }

                    if(max > activation){
                        switch(index)
                        {
                            case 0:
                                Accelerate(sim->P->Generation[i].car);
                                break;

                            case 1:
                                Brake(sim->P->Generation[i].car);
                                break;

                            default :
                                break;
                        }
                    }

                    max = 0;

                    for(int y = 2; y<4; y++)
                    {
                        if(sim->P->Generation[i].output[y] > max)
                        {
                            max = sim->P->Generation[i].output[y];
                            index = y;
                        }
                    }

                    if(max > activation){
                        switch(index)
                        {
                            case 2: 
                                if(sim->P->Generation[i].car->speed > 0.5)
                                {TurnLeft(sim->P->Generation[i].car);
                                rot+=1;}
                                break;
                            case 3: 
                                if(sim->P->Generation[i].car->speed > 0.5)
                                {TurnRight(sim->P->Generation[i].car);
                                rot-=1;}
                                break;
                            default:
                                break;
                        }
                    }

                    //update l'affichage 
                    if(accelerate%2 == 1)
                    {
                        delay(&ticks,60);
                    }
                    
                    Draw_Background(sim->disp->renderer);
                    Display_Info(sim,sim->P->nb_of_population,fitness_max);
                    Display_Button(sim->disp->renderer,0);
                    MoveForward(sim->P->Generation[i].car);
                    UpdateCar(sim->P->Generation[i].car);
                    Rotation(sim->disp->renderer,sim->P->Generation[i].car,img,rot*1.40625);
                    Draw_Circuit(sim->disp->renderer,sim->track);
                    ScoreUpdate(sim->P->Generation[i].car,sim->track);

                    count+=1;

                    if(count > 20 && sim->P->Generation[i].car->speed == 0)
                    {
                        sim->P->Generation[i].car->score = 0;
                        brk = 1;
                    }

                    if(count > 2000)
                        brk = 1;
                }
            }   
        }
    }

        int fitness = BestScore(sim->P);
        int index = BestCarIndex(sim->P);
        if(fitness > fitness_max){
            fitness_max = fitness;
            Set_Network(&sim->P->Generation[index],BestCar);
            index++;
        }        
        sim->P = Change_Population(sim->P,sim->img);
    }

    FreeSimulation(sim);
    FreeNetwork(BestCar);
    free(BestCar);

    }while(reset==0);
}