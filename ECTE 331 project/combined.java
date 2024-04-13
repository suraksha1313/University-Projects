package project331;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class combined { //main class
    private static final int numOfThreads = 4; // Number of threads to use

    public static void main(String[] args) {
    	
        // Single-threaded implementation
        long startTime = System.currentTimeMillis();
        singleThreadedEqualize();
        long endTime = System.currentTimeMillis();
        long singleTime = endTime - startTime;

        System.out.println("Single-threaded Execution Time: " + singleTime + "ms");

        // Multi-threaded implementation with shared histogram array
        startTime = System.currentTimeMillis();
        multiThreadedEqualize(); 
        endTime = System.currentTimeMillis();
        long multiTime = endTime - startTime;

        System.out.println("Multi-threaded Execution Time: "  + multiTime + "ms");
    }

    private static void singleThreadedEqualize() {  
        	
            try {
                // Load the image
                BufferedImage image = ImageIO.read(new File("C:\\Users\\surak\\Desktop\\Rain_Tree.jpg"));

                // Convert to grayscale
                BufferedImage grayImage = Grayscale(image);

               // Calculate histogram
                int[] histogram = calHistogram(grayImage);

                // Calculate cumulative histogram
                int[] cumulativeHist = calCumulative(histogram);

                // Equalize the image
                equalizeImage(grayImage, cumulativeHist);
          
                // Save the equalized image
                saveImage(grayImage, "C:\\Users\\surak\\Desktop\\single.jpg");
                 
            } catch (IOException e) {e.printStackTrace();}
            
        }

    private static void multiThreadedEqualize() {
    	 try {
             // Load the image
             BufferedImage image = ImageIO.read(new File("C:\\Users\\surak\\Desktop\\Rain_Tree.jpg"));

             // Convert to grayscale
             BufferedImage grayImage = Grayscale(image);

             // Divide the image into equal parts for each thread
             int width = grayImage.getWidth();
             int height = grayImage.getHeight();
             int subHeight = height / numOfThreads; //dividing height by number of threads for subheight

             // Create and start the threads
             Thread[] threads = new Thread[numOfThreads]; 
             
             for (int i = 0; i < numOfThreads; i++) {
                 final int startX = i * subHeight;
                 final int endX = (i == numOfThreads - 1) ? height : (i + 1) * subHeight; // conditional loop 

                 threads[i] = new Thread(new Runnable() {
                     @Override
                     public void run() {
                         // Process the assigned part of the image
                         for (int y = startX; y < endX; y++) {
                             for (int x = 0; x < width; x++) {
                                 int gPixel = grayImage.getRGB(x, y) & 0xFF;
                                 grayImage.setRGB(x, y, (gPixel << 16) + (gPixel << 8) + gPixel); //sets the RGB value to the pixel coordinates
                             }
                         }
                     }
                 });

                 threads[i].start();
             }

             // Wait for all threads to finish
             for (Thread thread : threads) {
                 thread.join();
             }
             
             // Calculate histogram
             int[] histogram = calHistogram(grayImage);

             // Calculate cumulative histogram
             int[] cumulativeHist = calCumulative(histogram);

             // Equalize the image
             equalizeImage(grayImage, cumulativeHist);
             
             // Save the equalized image
             saveImage(grayImage, "C:\\Users\\surak\\Desktop\\multi.jpg");
             
         } catch (IOException | InterruptedException e) {e.printStackTrace();}
     }
 

 // Convert the input image to grayscale
    private static BufferedImage Grayscale(BufferedImage image) {
        int width = image.getWidth(); //getting the width
        int height = image.getHeight(); //getting the height
        
        //creates a buffered image object grayImage
        BufferedImage grayImage = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

        // Convert each pixel to grayscale
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) { 
                Color color = new Color(image.getRGB(x, y));
                int red = color.getRed(); //getting the color red
                int green = color.getGreen(); //getting the color green
                int blue = color.getBlue(); //getting the color blue
                
             // Calculate grayscale value
                int gray = (red + green + blue) / 3; //average of RGB values
                int grayRGB = (gray << 16) + (gray << 8) + gray; // creating new grayscale pixel value

                grayImage.setRGB(x, y, grayRGB); //setting the pixel position in the grayimage 
            }
        }

        return grayImage; // returning grayscaled image
    }
    
 // Equalize the histogram of the grayscale image
    private static int[] calHistogram(BufferedImage image) {
        int width = image.getWidth();
        int height = image.getHeight();
        int[] histogram = new int[256]; // histogram array

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int gPixel = image.getRGB(x, y) & 0xFF; // getting the gray pixel value 
                histogram[gPixel]++;
            }
        }

        return histogram;
    }
  // Calculate the cumulative histogram
    private static int[] calCumulative(int[] histogram) {
        int[] cumulativeHist = new int[256]; // cumulative array 
        cumulativeHist[0] = histogram[0]; 

        for (int i = 1; i < 256; i++) {
            cumulativeHist[i] = cumulativeHist[i - 1] + histogram[i]; //calculating the cumulative value 
        }

        return cumulativeHist;
    }
 // Equalize the histogram of the grayscale image
    private static void equalizeImage(BufferedImage image, int[] cumulativeHist) {
        int width = image.getWidth(); // get the width
        int height = image.getHeight(); // get the height 
        int totalPixels = width * height; //multiplying the dimensions 

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int grayPixel = image.getRGB(x, y) & 0xFF; // calculating the gray pixel 
                int eqGray = (int) (cumulativeHist[grayPixel] * 255.0 / totalPixels); // calculating the equalized gray value
                image.setRGB(x, y, (eqGray << 16) + (eqGray << 8) + eqGray); // set RGB value to the image 
            }
        }
    }
 // Save the image to the specified output path
    private static void saveImage(BufferedImage image, String path) throws IOException {
        File output = new File(path); // creating a new output path for file
        ImageIO.write(image, "jpg", output); // saving the new image in the output path
    }
   
}
