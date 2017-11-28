/**
 * Created by aleksandrs on 11/27/17.
 */

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Tokenizer {

    // Store information on tokens
    private class TokenInfo{
        public final Pattern regex; // regex to match input string
        public final String tokenType; // type of the token (e.g. variable, continue, numstr, etc.)

        public TokenInfo(Pattern regex, String tokenType){
            super();
            this.regex = regex;
            this.tokenType = tokenType;
        }
    }

    private LinkedList<TokenInfo> tokenInfos; // store info on ALL the tokens
    public Tokenizer(){
        tokenInfos = new LinkedList<>();
        tokens = new LinkedList<>();
    }
    // Add new token info to the list
    public void add(String regex, String tokenType){
        // ^ means start of line in this case
        // we consider regexes we input. plus repeats brackets (presumably)
        tokenInfos.add(new TokenInfo(Pattern.compile("^("+regex+")"), tokenType));
    }

    public class Token{
        public final String tokenType;
        public final String sequence;

        public Token(String tokenType, String sequence){
            super();
            this.tokenType = tokenType;
            this.sequence = sequence;
        }
    }

    private LinkedList<Token> tokens;

    // read file into string
    static String readFile(String testFileName) throws IOException{
        BufferedReader bufferedReader = new BufferedReader(new FileReader(testFileName));
        try {
            StringBuilder stringBuilder = new StringBuilder();
            String line = bufferedReader.readLine();
            while (line != null){
                stringBuilder.append(line);
                stringBuilder.append("\n");
                line = bufferedReader.readLine();
            }
            return stringBuilder.toString();
        }
        finally {
            bufferedReader.close();
        }


    }
    public static void main(String[] args) {
        Tokenizer tokenizer = new Tokenizer();
        // NOTE: precedence of adding token regexes is important. Things added first are tested FIRST.
        // this also applies to order in which things are added to one token type
        tokenizer.add("int","init");
        tokenizer.add("if|else|while","test"); // test
        tokenizer.add("true|false", "bool"); // bool
        tokenizer.add("not", "not"); // negation operator
        tokenizer.add("break|continue", "brcont"); // break or continue
        tokenizer.add("read|write", "readwrite"); // read or write
        tokenizer.add("-?[0-9]+","num"); // integer number (possible negative)
        tokenizer.add("-?[a-zA-Z][a-zA-Z0-9_]*", "var"); // variable (possible negative)
        tokenizer.add("!=|==|<=|>=|>|<","compop"); // comparison operator (for tests)
        tokenizer.add("\\&|\\|","binop"); // binary operator
        tokenizer.add("=","equals"); // equals sign
        tokenizer.add("\\;","semicolon"); // semicolon
        tokenizer.add("\\(", "opbr"); // open bracket
        tokenizer.add("\\)", "clbr"); // close bracket
        tokenizer.add("\\{", "opcurlbr"); // open curly bracket
        tokenizer.add("\\}", "clcurlbr"); // close curly bracket
        tokenizer.add("\\[", "opsqbr"); // open square bracket
        tokenizer.add("\\]", "clsqbr"); // close square bracket
        tokenizer.add("[+-]", "plumin"); // plus or minus
        tokenizer.add("[*/]", "muldiv"); // mult or divide


        String testFileName = "test.microC"; // name of file to test and tokenize
        String testString = "";
        try {
            testString = readFile(testFileName);
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println(testString);

//        try {
//            tokenizer.tokenize("int x;int c; x = -10;while(x > 0){x = -a;read(c); if(c==0){break;}}write(x);");
//
//            for (Tokenizer.Token token : tokenizer.getTokens()) {
//                System.out.println("" + token.tokenType + " : " + token.sequence);
//            }
//        }
//        catch (RuntimeException e) {
//            System.out.println(e.getMessage());
//        }
        try {
            tokenizer.tokenize(testString);

            for (Tokenizer.Token token : tokenizer.getTokens()) {
                System.out.println("" + token.tokenType + " : " + token.sequence);
            }
        }
        catch (RuntimeException e) {
            System.out.println(e.getMessage());
        }
    }

    public void tokenize(String string){
        String s = string.trim(); // this will contain the input string without leading/trailing spaces
        tokens.clear(); // clear it from old data

        while (!s.equals("")){
            boolean match = false;

            for (TokenInfo info: tokenInfos){
                Matcher matcher = info.regex.matcher(s); // trying to match given string against given pattern
                if (matcher.find()){
                    match = true;

                    String token = matcher.group().trim(); // returns the last matched string
                    tokens.add(new Token(info.tokenType, token)); // add token to the list with corresponding type

                    s = matcher.replaceFirst(""); // replaces the last matched string by ""
                    s = s.trim(); // and trims it to remove the space at the start of the string
                    break;
                }
            }
            if (!match){
                throw new RuntimeException("Unexpected character in input: " + s);
            }
        }
    }

    public LinkedList<Token> getTokens(){
        return tokens;
    }

}
