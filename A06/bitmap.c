/*----------------------------------------------
 * Author: Gabrielle Stewart 
 * Date: October 2024
 * Description: This program reads in a single 64-bit unsigned integer and outputs it as an 8x8 1bpp sprite.
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);

  // todo: your code here

  for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Calculate the bit position we want to check
            int bit_position = 63 - (row * 8 + col);
            unsigned long mask = 0x1ul << bit_position;

            // Check if the bit at the current position is 1 or 0
            if (img & mask) {
                printf("@");
            } else {
                printf(" ");
            }
        }
        // Move to the next row
        printf("\n");
    }
  return 0;
}
