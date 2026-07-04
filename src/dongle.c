#include "codexion.h"

int take_dongle_use_in(t_dongle* dongle){
    pthread_mutex_lock(&(dongle->mutex));
    if(dongle->take_in_use==0)
        dongle->take_in_use = 1;
    else{

        pthread_mutex_unlock(&(dongle->mutex));
        return 0;
    }
    pthread_mutex_unlock(&(dongle->mutex));
    return 1;
}

int add_dongle_que(t_dongle* dongle,t_coder* coder){
    if(take_dongle_use_in(dongle)){
        return 1;
    }else{
        //ここにdequeにcoderをaddする
        coder->id=coder->id;
        
    }
    return 1;
}

