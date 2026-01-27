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

/* get color channels */
void colorChannels(char color[SLEN], 
            unsigned char *rVal,
            unsigned char *gVal,
            unsigned char *bVal);

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
            case 6:
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
                printf("Please input the border color: ");
                char color[SLEN];
                scanf("%s", color);
                printf("Please input the border width: ");
                int border_width;
                scanf("%d", &border_width);
                AddBorder(R, G, B, color, border_width);
                break;
            case 11:
                AutoTest(R, G, B);
                break;
            case 12:
                printf("Exiting the program.\n");
                break; 
            default:
                printf("Invalid option. Please try again.\n");
        }


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


/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

/* Print Menu Function */
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
}

/* Change image to black & white */
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
/* Invert colors (negative function)*/
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
/* Custom color filter function */
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

                    /* Given equations */

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

void Edge(unsigned char R[WIDTH][HEIGHT],
	  unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT])
    {
        unsigned char tempR[WIDTH][HEIGHT];
        unsigned char tempG[WIDTH][HEIGHT];
        unsigned char tempB[WIDTH][HEIGHT];

        int x, y;
        for (y = 1; y < HEIGHT - 1; y++){
            for (x = 1; x < WIDTH - 1; x++){
                /* Finding Neighboring pixels */
                int RedA = R[x-1][y-1], RedB = R[x][y-1], RedC = R[x+1][y-1];
                int RedD = R[x-1][y],   RedE = R[x][y],   RedF = R[x+1][y];
                int RedG = R[x-1][y+1], RedH = R[x][y+1], RedI = R[x+1][y+1];

                /* Given equations and repeated logic */
                int satRed = -RedA - RedB - RedC - RedD + 8*RedE - RedF - RedG - RedH - RedI;
                if (satRed < 0) satRed = 0;
                if (satRed > 255) satRed = 255;
                tempR[x][y] = (unsigned char)satRed;

                int GreenA = G[x-1][y-1], GreenB = G[x][y-1], GreenC = G[x+1][y-1];
                int GreenD = G[x-1][y],   GreenE = G[x][y],   GreenF = G[x+1][y];
                int GreenG = G[x-1][y+1], GreenH = G[x][y+1], GreenI = G[x+1][y+1];

                int satGreen = -GreenA - GreenB - GreenC - GreenD + 8*GreenE - GreenF - GreenG - GreenH - GreenI;
                if (satGreen < 0) satGreen = 0;
                if (satGreen > 255) satGreen = 255;
                tempG[x][y] = (unsigned char)satGreen;

                int BlueA = B[x-1][y-1], BlueB = B[x][y-1], BlueC = B[x+1][y-1];
                int BlueD = B[x-1][y],   BlueE = B[x][y],   BlueF = B[x+1][y];
                int BlueG = B[x-1][y+1], BlueH = B[x][y+1], BlueI = B[x+1][y+1];

                int satBlue = -BlueA - BlueB - BlueC - BlueD + 8*BlueE - BlueF - BlueG - BlueH - BlueI;
                if (satBlue < 0) satBlue = 0;
                if (satBlue > 255) satBlue = 255;
                tempB[x][y] = (unsigned char)satBlue;

            }

        }
        /* Copy temp back to original*/
        for (y = 0; y < HEIGHT; y++){
            for (x = 0; x < WIDTH; x++){
                R[x][y] = tempR[x][y];
                G[x][y] = tempG[x][y];
                B[x][y] = tempB[x][y];
            }
        }
        printf("Edge detection operation is done!\n");
    }

void Shuffle(unsigned char R[WIDTH][HEIGHT],
        unsigned char G[WIDTH][HEIGHT],
            unsigned char B[WIDTH][HEIGHT])
        {
            int newWidth = WIDTH / 4;
            int newHeight = HEIGHT / 4;
            
            int x, y;
            for (int i = 0; i < 8; i++){
                int blockA = i;
                int blockB = 15 - i;

                int rowA = blockA / 4;
                int colA = blockA % 4;
                int rowB = blockB / 4;
                int colB = blockB % 4;

                int Ax = colA * newWidth;
                int Ay = rowA * newHeight;
                int Bx = colB * newWidth;
                int By = rowB * newHeight;

                for (y = 0; y < newHeight; y++){
                    for (x = 0; x < newWidth; x++){
                        int ax = Ax + x, ay = Ay + y;
                        int bx = Bx + x, by = By + y;

                        unsigned char temp;

                        temp = R[ax][ay]; R[ax][ay] = R[bx][by]; R[bx][by] = temp;
                        temp = G[ax][ay]; G[ax][ay] = G[bx][by]; G[bx][by] = temp;
                        temp = B[ax][ay]; B[ax][ay] = B[bx][by]; B[bx][by] = temp;
                    }
                }
            }
            printf("Shuffle operation is done!\n");
        }

void HFlip(unsigned char R[WIDTH][HEIGHT],
    unsigned char G[WIDTH][HEIGHT],
    unsigned char B[WIDTH][HEIGHT])
    /* TWO POINTER IMPLEMENTATION!!!! Leetcode is paying off*/
    {
        for (int i = 0; i < HEIGHT; i++){
            int left = 0;
            int right = WIDTH - 1;

            while (left < right){
                unsigned char temp;

                temp = R[left][i]; 
                R[left][i] = R[right][i]; 
                R[right][i] = temp;
                
                temp = G[left][i]; 
                G[left][i] = G[right][i]; 
                G[right][i] = temp;
                
                temp = B[left][i]; 
                B[left][i] = B[right][i]; 
                B[right][i] = temp;

                left++;
                right--;
            }
        }
        printf("HFlip operation is done!\n");
    }

void VMirror(unsigned char R[WIDTH][HEIGHT],
        unsigned char G[WIDTH][HEIGHT],
            unsigned char B[WIDTH][HEIGHT])
    {
        int x, y;
        for (y = 0; y < HEIGHT / 2; y++){
            for (x = 0; x <WIDTH; x++){
                int mirror = HEIGHT -1 -y;

                R[x][mirror] = R[x][y];
                G[x][mirror] = G[x][y];
                B[x][mirror] = B[x][y];
            }
        }
        printf("VMirror operation is done!\n");
    }

void AddBorder(unsigned char R[WIDTH][HEIGHT],
	       unsigned char G[WIDTH][HEIGHT],
               unsigned char B[WIDTH][HEIGHT],
	       char color[SLEN], int border_width)
    {
        unsigned char rVal = 0, gVal = 0, bVal = 0;
        colorChannels(color, &rVal, &gVal, &bVal);
        int x, y;
        for (y = 0; y < HEIGHT; y++){
            for (x = 0; x < WIDTH; x++){
                if (x < border_width || x >= WIDTH - border_width ||
                    y < border_width || y >= HEIGHT - border_width){
                        R[x][y] = rVal;
                        G[x][y] = gVal;
                        B[x][y] = bVal;
                    }
            }
        }
        printf("Add Border operation is done!\n");
    }

void colorChannels(char color[SLEN], 
            unsigned char *rVal,
	        unsigned char *gVal,
            unsigned char *bVal)
{
    *rVal = 0; *gVal = 0; *bVal = 0;

    if (strcmp(color, "black") == 0){
        *rVal = 0; *gVal = 0; *bVal = 0;
    }
    else if (strcmp(color, "white") == 0){
        *rVal = 255; *gVal = 255; *bVal = 255;
    }
    else if (strcmp(color, "red") == 0){
        *rVal = 255; *gVal = 0; *bVal = 0;
    }
    else if (strcmp(color, "green") == 0){
        *rVal = 0; *gVal = 255; *bVal = 0;
    }
    else if (strcmp(color, "blue") == 0){
        *rVal = 0; *gVal = 0; *bVal = 255;
    }
    else if (strcmp(color, "yellow") == 0){
        *rVal = 255; *gVal = 255; *bVal = 0;
    }
    else if (strcmp(color, "orange") == 0){
        *rVal = 255; *gVal = 165; *bVal = 0;
    }
    else if (strcmp(color, "cyan") == 0){
        *rVal = 0; *gVal = 255; *bVal = 255;
    }
    else if (strcmp(color, "pink") == 0){
        *rVal = 255; *gVal = 190; *bVal = 200;
    }
    else {
        *rVal = 0; *gVal = 0; *bVal = 0; // default to black
    }
}

/* EOF */
