#include <stdio.h>
#include <stdlib.h>
 
static void burn_cpu() {
  unsigned int acc = 0;
  for (int i = 0; i < 1048576; ++i) {
    acc += random() * 65536;
  }
}
 
int main(int argc, char** argv) {
  for (int i = 0; i < 32; ++i) {
    burn_cpu();
  }
 
  return 0;
}
