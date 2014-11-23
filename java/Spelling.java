import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class Spelling {
    public static void main(String []args) {
        Corrector c = new Corrector();
        c.train("../big.txt");
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        String input = null;

        while (true){
            try {
                input = br.readLine();
                System.out.println(input + "-->" + c.correct(input));
            }
            catch (IOException ioe) {
                System.out.println("IO error");
                System.exit(1);
            }
        }
    }
}
