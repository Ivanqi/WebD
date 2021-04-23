#include "src/Entry.h"
using namespace webd;

extern char favicon[555];

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("配置文件不存在，请载入配置文件!\n");
        return 0;
    }

    Entry enter;
    enter.start(argv);

    return 0;
}
