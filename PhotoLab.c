/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2026                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** global definitions ***/
#define WIDTH  640		/* image width */
#define HEIGHT 360		/* image height */
#define SLEN    80		/* maximum length of file names */

/*** function declarations ***/


/* print a menu */
void PrintMenu(void);

/* read image from a file */
int LoadImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
		 unsigned char B[WIDTH][HEIGHT]);

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT],
		 int target_r, int target_g, int target_b, int threshold,
		 int replace_r, int replace_g, int replace_b);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT],
	  unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT]);

/* mirror image Vertically */
void VMirror(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);


/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT],
	       unsigned char G[WIDTH][HEIGHT],
               unsigned char B[WIDTH][HEIGHT],
	       char color[SLEN], int border_width);

/* flip image horizontally */
void HFlip(unsigned char R[WIDTH][HEIGHT],
	   unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT]);

/* test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];

    /* Please replace the following code with proper menu with function calls for DIP operations */
    int option = 0;

    while (option != 12){
        PrintMenu();
        scanf("%d", &option);

        switch (option){
            case 1: {
                printf("Please input the file name to load: ");
                char fname[SLEN];
                scanf("%s", fname);
                LoadImage(fname, R, G, B);
                break;
            }
            case 2: {
                printf("Please input the file name to save: ");
                char sname[SLEN];
                scanf("%s", sname);
                SaveImage(sname, R, G, B);
                break;
            }
            case 3: {
                BlackNWhite(R, G, B);
                break;
            }
            case 4: {
                Negative(R, G, B);
                break;
            }
            case 5: 
                {
                int target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b;
                printf("Enter Red component for the target color: ");
                scanf("%d", &target_r);
                printf("Enter Green component for the target color: ");
                scanf("%d", &target_g);
                printf("Enter Blue component for the target color: ");
                scanf("%d", &target_b);
                printf("Enter threshold for the color difference: ");
                scanf("%d", &threshold);
                printf("Enter value Red component in the target color: ");
                scanf("%d", &replace_r);
                printf("Enter value Green component in the target color: ");
                scanf("%d", &replace_g);
                printf("Enter value Blue component in the target color: ");
                scanf("%d", &replace_b);
                
                ColorFilter(R, G, B, target_r, target_g, target_b, threshold,
                replace_r, replace_g, replace_b);
                break;
            }
            /*case 6:
                Edge(R, G, B);
                break;
            case 7:
                Shuffle(R, G, B);
                break;
            case 8:
                HFlip(R, G, B);
                break;
            case 9:
                VMirror(R, G, B);
                break;
            case 10:
                AddBorder(R, G, B, "orange", 24);
                break;
            case 11:
                AutoTest(R, G, B);
                break;
            case 12:
                printf("Exiting the program.\n");
                break; */
            default:
                printf("Invalid option. Please try again.\n");
        }


    }

    if (LoadImage("EngHall", R, G, B) != 0)
    {
      return 1;
    }

    /* End of replacing */

    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File) {
        printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH) {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT) {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File) {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_ext);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/*DO NOT EDIT AUTOTEST*/
/* Uncomment AutoTest() to run all DIP functions automatically. */
/*
void AutoTest(unsigned char R[WIDTH][HEIGHT],
              unsigned char G[WIDTH][HEIGHT],
              unsigned char B[WIDTH][HEIGHT])
{
    LoadImage("EngHall", R, G, B);
    Negative(R, G, B);
    SaveImage("negative", R, G, B);
    printf("Negative tested!\n\n");

    LoadImage("EngHall", R, G, B);
    ColorFilter(R, G, B, 152, 135, 104, 20, 255, 0, 0);
    SaveImage("colorfilter", R, G, B);
    printf("Color Filter tested!\n\n");

    LoadImage("EngHall", R, G, B);
    Edge(R, G, B);
    SaveImage("edge", R, G, B);
    printf("Edge Detection tested!\n\n");

    LoadImage("EngHall", R, G, B);
    VMirror(R, G, B);
    SaveImage("vmirror", R, G, B);
    printf("VMirror tested!\n\n");

    LoadImage("EngHall", R, G, B);
    AddBorder(R, G, B, "orange", 24);
    SaveImage("border", R, G, B);
    printf("Border tested!\n\n");

    LoadImage("EngHall", R, G, B);
    BlackNWhite(R, G, B);
    SaveImage("bw", R, G, B);
    printf("Black & White tested!\n\n");

    LoadImage("EngHall", R, G, B);
    HFlip(R, G, B);
    SaveImage("hflip", R, G, B);
    printf("HFlip tested!\n\n");

    LoadImage("EngHall", R, G, B);
    Shuffle(R, G, B);
    SaveImage("shuffle", R, G, B);
    printf("Shuffle tested!\n\n");
}
*/

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

void PrintMenu(void){
    printf("--------------------------------\n");
    printf("1: Load a PPM image\n");
    printf("2: Save an image in PPM and JPEG format\n");
    printf("3: Change a color image to Black & White\n");
    printf("4: Make a negative of an image\n");
    printf("5: Color filter an image\n");
    printf("6: Sketch the edge of an image\n");
    printf("7: Shuffle an image\n");
    printf("8: Flip an image horizontally\n");
    printf("9: Mirror an image vertically\n");
    printf("10: Add Border to an image\n");
    printf("11: Test all functions\n");
    printf("12: Exit\n");
    printf("please make your choice: ");
    fflush(stdout);
}

void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
		 unsigned char B[WIDTH][HEIGHT])
    {
        int x, y;
        unsigned char gray;
        for (y = 0; y < HEIGHT; y++) {
            for (x = 0; x < WIDTH; x++) {
                gray = (unsigned char)((R[x][y] + G[x][y] + B[x][y]) / 3);
                R[x][y] = gray;
                G[x][y] = gray;
                B[x][y] = gray;
            }
        }
        printf("Black & White operation is done!\n");
    }

void Negative(unsigned char R[WIDTH][HEIGHT],
        unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT]) 
    {
        int x, y;
        for (y = 0; y < HEIGHT; y++) {
            for (x = 0; x < WIDTH; x++) {
                R[x][y] = 255 - R[x][y];
                G[x][y] = 255 - G[x][y];
                B[x][y] = 255 - B[x][y];
            }
        }
        printf("Negative operation is done!\n");
    }

void ColorFilter(unsigned char R[WIDTH][HEIGHT],
        unsigned char G[WIDTH][HEIGHT],
        unsigned char B[WIDTH][HEIGHT],
        int target_r, int target_g, int target_b, int threshold,
        int replace_r, int replace_g, int replace_b)
        {
            int x, y;
            for (y = 0; y < HEIGHT; y++){
                for (x = 0; x < WIDTH; x++){    
                    int currR = R[x][y];
                    int currG = G[x][y];
                    int currB = B[x][y];

                    if (currR >= (target_r - threshold) && currR <= (target_r + threshold) &&
                        currG >= (target_g - threshold) && currG <= (target_g + threshold) &&
                        currB >= (target_b - threshold) && currB <= (target_b + threshold)){
                            R[x][y] = replace_r;    
                            G[x][y] = replace_g;
                            B[x][y] = replace_b;
                        }
                        
                }
            }
            printf("Color filter operation is done!\n");
        }
        
    
/* EOF */
