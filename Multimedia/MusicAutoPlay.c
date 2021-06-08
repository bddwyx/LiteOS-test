/**
  ******************************************************************************
  * @FileName			    MusicAutoPlay.cpp
  * @Description            I'm insane
  * @author                 Wang Xi
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#define __MUSICAUTOPLAY_C

#include "Lib_songs.h"

soft_timer_s timer_song_1ms;

/**
 * @brief Initialize a music task and initialize its soft timer.
 * @param timer_1ms The soft timer for music auto-play task, defined in SoftTimer.c.
 * @param t_song Script of the song.
 * @param task_address The pointer of the pointer to the music task. Can be [(task_s**)0] under cases where the function is called by user or there's no need to know which task music auto-play is.
 * @return The state of the initialize procedure.
 *      @arg 0 Fail
 *      @arg 1 Succeed
 */
uint8_t Music_init(soft_timer_s* timer_1ms, T_Song* t_song, task_s** task_address){
    if(t_song->first_enter){
        soft_timer_init(timer_1ms, 1);
        t_song->first_enter = 0;
    }	//Init soft_timer

    PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    return soft_timer_add_task(timer_1ms, &Music_auto_play, (void*)(t_song), 1, task_address);
}

/**
 * @brief Cancel a music task.
 * @param auto_play_task The task pointer of music auto-play.
 * @param t_song Script of the song.
 * @return none
 */
void Music_deinit(task_s* auto_play_task, T_Song* t_song){
    t_song->note_index = 0;
    t_song->first_enter = 1;

    PWMGenDisable(PWM0_BASE, PWM_GEN_3);
    soft_timer_delete_task(auto_play_task);
}

/**
 * @brief Music auto-play task to be added into soft timer.
 * @param auto_play_task The task pointer of music auto-play.
 * @param args The pointer to arg(s). In this situation, it should be transformed into T_song*, representing the script.
 */
void Music_auto_play(task_s* auto_play_task, void* args){
    static uint16_t note_interval = 0;
    T_Song* t_song=(T_Song*)args;

    if(0 == note_interval){ //load a new note
        uint16_t note = t_song->t_script[t_song->note_index].Note;
        note_interval = t_song->t_script[t_song->note_index].Time;

        // Control the buzzeer
        if(0 == note){
            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, 0);
        } else {
            PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, SYSTEMCLOCK / note);
            PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, SYSTEMCLOCK / 2 / note);
        }

        // Music final action
        if(++(t_song->note_index) >= t_song->note_num){
            Music_deinit(auto_play_task, t_song);
            note_interval = 0;
        }
    }

    else{
        note_interval--;
    }
}

void Music_schedule(){
    soft_timer_schedule(&timer_song_1ms);
}
