#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include "libjpeg-turbo/turbojpeg.h"

int jpeg_to_fd(int fd, void* pixels, int width, int height, int pitch)
{
    static unsigned char * jpegbuf = NULL;
    static unsigned long jpegsize = 0;
    static tjhandle tjc = tjInitCompress();
    int ret;
    if(!tjCompress2(tjc, (const unsigned char*)pixels, width, pitch, height, 0,
                &jpegbuf, &jpegsize, TJSAMP_420, 80, 0))
    {
        perror("tjCompress2");
        return -1;
    }
    ret = write(fd, jpegbuf, jpegsize);
    if(ret < 0)
    {
        perror("write to fd");
        return -1;
    }
    return ret;
}

void socketloop()
{

}

int main(int argc, char** argv)
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;

    const char* file = "/dev/graphics/fb0";

    if(argc > 1)
        file = argv[1];

    // Open the file for reading and writing
    fbfd = open(file, O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    printf("%s (%s) = %i\n", "xres", "visible resolution", vinfo.xres);
    printf("%s (%s) = %i\n", "yres", "visible resolution", vinfo.yres);
    printf("%s (%s) = %i\n", "xres_virtual", "virtual resolution", vinfo.xres_virtual);
    printf("%s (%s) = %i\n", "yres_virtual", "virtual resolution", vinfo.yres_virtual);
    printf("%s (%s) = %i\n", "xoffset", "offset from virtual to visible", vinfo.xoffset);
    printf("%s (%s) = %i\n", "yoffset", "resolution", vinfo.yoffset);
    printf("%s (%s) = %i\n", "bits_per_pixel", "guess what", vinfo.bits_per_pixel);
    printf("%s (%s) = %i\n", "grayscale", "0 = color, 1 = grayscale,", vinfo.grayscale);
    printf("%s (%s) = %i\n", "nonstd", "!= 0 Non standard pixel format", vinfo.nonstd);
    printf("%s (%s) = %i\n", "activate", "see FB_ACTIVATE_*", vinfo.activate);
    printf("%s (%s) = %i\n", "height", "height of picture in mm", vinfo.height);
    printf("%s (%s) = %i\n", "width", "width of picture in mm", vinfo.width);
    printf("%s (%s) = %i\n", "accel_flags", "(OBSOLETE) see fb_info.flags", vinfo.accel_flags);
    printf("%s (%s) = %i\n", "pixclock", "pixel clock in ps (pico seconds)", vinfo.pixclock);
    printf("%s (%s) = %i\n", "left_margin", "time from sync to picture", vinfo.left_margin);
    printf("%s (%s) = %i\n", "right_margin", "time from picture to sync", vinfo.right_margin);
    printf("%s (%s) = %i\n", "upper_margin", "time from sync to picture", vinfo.upper_margin);
    printf("%s (%s) = %i\n", "lower_margin", "time from sync to picture", vinfo.lower_margin);
    printf("%s (%s) = %i\n", "hsync_len", "length of horizontal sync", vinfo.hsync_len);
    printf("%s (%s) = %i\n", "vsync_len", "length of vertical sync", vinfo.vsync_len);
    printf("%s (%s) = %i\n", "sync", "see FB_SYNC_*", vinfo.sync);
    printf("%s (%s) = %i\n", "vmode", "see FB_VMODE_*", vinfo.vmode);
    printf("%s (%s) = %i\n", "rotate", "angle we rotate counter clockwise", vinfo.rotate);
    //printf("%s (%s) = %i\n", "colorspace", "colorspace", vinfo.colorspace);

    printf("%s (%s) = %16s\n", "id", "identification string", finfo.id);
    printf("%s (%s) = %lx\n", "smem_start", "Start of frame buffer mem", finfo.smem_start);
    printf("%s (%s) = %i\n", "smem_len", "Length of frame buffer mem", finfo.smem_len);
    printf("%s (%s) = %i\n", "type", "see FB_TYPE_*	", finfo.type);
    printf("%s (%s) = %i\n", "type_aux", "Interleave for interleaved Planes", finfo.type_aux);
    printf("%s (%s) = %i\n", "visual", "see FB_VISUAL_*	", finfo.visual);
    printf("%s (%s) = %i\n", "xpanstep", "zero if no hardware panning ", finfo.xpanstep);
    printf("%s (%s) = %i\n", "ypanstep", "zero if no hardware panning ", finfo.ypanstep);
    printf("%s (%s) = %i\n", "ywrapstep", "zero if no hardware ywrap   ", finfo.ywrapstep);
    printf("%s (%s) = %i\n", "line_length", "length of a line in bytes   ", finfo.line_length);
    printf("%s (%s) = %lu\n", "mmio_start", "Start of Memory Mapped I/O", finfo.mmio_start);
    printf("%s (%s) = %i\n", "mmio_len", "Length of Memory Mapped I/O ", finfo.mmio_len);
    printf("%s (%s) = %i\n", "accel", "Indicate to driver which", finfo.accel);
    //printf("%s (%s) = %i\n", "capabilities", "see FB_CAP_*		", finfo.capabilities);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres_virtual * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((long)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

//    x = 100; y = 100;       // Where we are going to put the pixel

//    // Figure out where in memory to put the pixel
//    for (y = 100; y < 300; y++)
//        for (x = 100; x < 300; x++) {

//            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
//                       (y+vinfo.yoffset) * finfo.line_length;

//            if (vinfo.bits_per_pixel == 32) {
//                *(fbp + location) = 100;        // Some blue
//                *(fbp + location + 1) = 15+(x-100)/2;     // A little green
//                *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
//                *(fbp + location + 3) = 0;      // No transparency
//        //location += 4;
//            } else  { //assume 16bpp
//                int b = 10;
//                int g = (x-100)/6;     // A little green
//                int r = 31-(y-100)/16;    // A lot of red
//                unsigned short int t = r<<11 | g << 5 | b;
//                *((unsigned short int*)(fbp + location)) = t;
//            }

//        }
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
