#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

long long start_time; // プログラム開始時刻を保持


long long get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // 秒をミリ秒に直し、マイクロ秒をミリ秒にして足し合わせる
    return ((long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
void* coder_routine(void* arg) {
    int id = *(int*)arg;
    
    // 現在の経過時間を計算
    long long now = get_time_ms() - start_time;
    printf("%lld ms: コーダー %d が作業を開始しました\n", now, id);
    
    usleep(50000); // 50ミリ秒ほど作業（シミュレーション）
    
    now = get_time_ms() - start_time;
    printf("%lld ms: コーダー %d が終了しました\n", now, id);
    
    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];

    start_time = get_time_ms(); // 1. シミュレーション開始時間を記録

    for (int i = 0; i < 5; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, coder_routine, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}