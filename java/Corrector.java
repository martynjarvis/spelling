import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.ArrayList;

public class Corrector {

    private Map<String,Integer> word_count;

    public Corrector(){
        word_count = new HashMap<String,Integer>();
    }

    public void train(String filename){
        System.out.println("Training.");
        Pattern word_regex = Pattern.compile("[a-zA-Z]+");
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(filename));
        }
        catch(FileNotFoundException e) {
            System.out.println(e.getMessage());
            System.exit(-1);
        }
        String line;
        try {
            while ((line = br.readLine()) != null) {
                // process the line.
                Matcher matcher = word_regex.matcher(line);
                while (matcher.find()) {
                    String key = matcher.group();
                    if (word_count.containsKey(key)){
                        word_count.put(key, word_count.get(key) + 1);
                    }
                    else {
                        word_count.put(key, 1);
                    }
                }
            }
        }
        catch(IOException e) {
            System.out.println(e.getMessage());
        }
        try {
            br.close();
        }
        catch(IOException e) {
            System.out.println(e.getMessage());
        }
        System.out.println("Training done.");
        return;
    }

    public String correct(String word){
        if (word_count.containsKey(word)){
            return word;
        }

        String best_match = null;
        int max_val = 0;
        ArrayList<String> first_edits = edits(word);
        for( String first_edit : first_edits){
            if (word_count.containsKey(first_edit)){
                if (word_count.get(first_edit) > max_val) {
                    max_val = word_count.get(first_edit);
                    best_match = first_edit;
                }
            }
        }
        if (max_val > 0){
            return best_match;
        }
        return "FAIL";
    }

    private ArrayList<String> edits(String word){
        ArrayList<String> new_words = new ArrayList<String>();
        char[] alphabet = "abcdefghijklmnopqrstuvwxyz".toCharArray();
        // delete
        for (int i=1; i<=word.length(); i++){
            String a = word.substring(0,i-1);
            String b = word.substring(i);
            new_words.add(a+b);
        }
        // transpose
        for (int i=1; i<word.length(); i++){
            String a = word.substring(0,i-1);
            String b = word.substring(i-1,i);
            String c = word.substring(i,i+1);
            String d = word.substring(i+1);
            new_words.add(a+c+b+d);
        }
        // replace
        for (int i=1; i<=word.length(); i++){
            String a = word.substring(0,i-1);
            String b = word.substring(i);
            for( char c : alphabet) {
                new_words.add(a+c+b);
            }
        }
        // insert
        for (int i=0; i<=word.length(); i++){
            String a = word.substring(0,i);
            String b = word.substring(i);
            for( char c : alphabet) {
                new_words.add(a+c+b);
            }
        }
        return new_words;
    }
}
