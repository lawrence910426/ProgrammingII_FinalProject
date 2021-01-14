#include <thread>
#include <chrono>

#include "ReinLearn.h"
#include "client.h"

board *status = new board();
ReinLearn *AI = new ReinLearn();
Client *client = new Client("127.0.0.1", 7122);

void ListenLoop() { while(client->handle()) ; }

std::thread *loop;
int main() {
    loop = new std::thread(ListenLoop);
    std::string s = "Deep Q Learning AI (Artificial Idiot).";
    client->SendRegister(s);
    while(!client->should_start_game) ;
    while(true) {
        int tetro_type = rand() % 7;
        tetromino *T = new tetromino(tetro_type);
        int ans = AI->Get(status, T);
        int direction = ans / 10, offset = ans % 10;
        int reward;
        if(status->drop_tetro(T->Get(direction), offset, &reward)) break;
        for(int i = 0;i < 20;i++) {
            for(int j = 0;j < 10;j++) std::cout << status->show[i][j] << " ";
            std::cout << std::endl;
        }
        std::cout << "---------------------------" << std::endl;
        client->SendUpdateBoard(status->show);
        if(reward > 0) client->SendAttack(0, reward);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        delete T;
    }
    client->SendDead();
}
