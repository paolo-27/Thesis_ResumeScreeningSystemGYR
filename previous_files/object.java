import java.util.Scanner;
public class object {
    Scanner fer = new Scanner(System.in);
    int width;
    int height;    
    
    // Method to calculate the area of the object
    public int calculateArea() {
        System.out.print("Enter width: ");
        width = fer.nextInt();
        System.out.print("Enter height: ");
        height = fer.nextInt();
        return width * height;
    }

}
