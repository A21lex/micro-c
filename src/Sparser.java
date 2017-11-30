/**
 * Created by aleksandrs on 11/28/17.
 */

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * A special kind of parser to create a "folded ast" structure used within the project
 */
public class Sparser {

    public static void main(String[] args){
        Node tree = new Node("root","root");
        Node.printTree(tree, "-");

        Sparser sparser = new Sparser();
        ArrayList<String> tokens = sparser.getTokens("test.microC");
        System.out.println("tokens = " + tokens);

        Node testTree = sparser.sparse(tokens);
        Node.printTree(testTree, "="); // regular way (double for next level)
        Node.testPrintTree(testTree, 0); // tree level way (as many signs as level of the node)
    }

    // read file into string
    private static String readFile(String testFileName) throws IOException{
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

    private ArrayList<String> getTokens(String testFileName){
        ArrayList<String> tokens = new ArrayList<>(); // store tokens
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

        String testString = "";
        try {
            testString = readFile(testFileName);
        } catch (IOException e) {
            e.printStackTrace();
        }
        try{
            tokenizer.tokenize(testString);
            for (Tokenizer.Token token: tokenizer.getTokens()){
                if(token.tokenType.equals("var")){
                    tokens.add("$"+token.sequence); // MARK variables with dollar. Will be useful later.
                }
                else {
                    tokens.add(token.sequence);
                }
            }
        }
        catch (RuntimeException ex){
            ex.printStackTrace();
        }
        return tokens;
    }

    // Sparse the tokens. Returns folded tree.
    public Node sparse(ArrayList<String> tokens){
    Node rootNode = new Node("root","root"); // start with the root - can always find it as parent is NULL
    Node placeHolderNode = rootNode; // some placeholder node to keep other nodes we are interested in
    int c = 0; // counter
    StringBuilder sb = new StringBuilder(); // building strings to add to the tree
    String type = ""; // store type of node we add to the tree
    // state 0 will mean adding to "current root" (placeHolderNode)
    int state = 1; // start in state 1

        while (c < tokens.size()){
            switch (state){

                case 0:
                    // adding stuff to current root
                    System.out.println("add stuff to root");
                    System.out.println("adding->" + sb.toString().replace("$",""));
                    String stringToAdd = sb.toString().replace("$","");
                    placeHolderNode.addChild(new Node(stringToAdd,type));
                    sb = new StringBuilder(); // clear the string builder
                    type = "";
                    state = 1;
                    break;
                case 1:
                    // this is DEFINITELY initialization
                    // var or array? Does not matter, use while until semicolon
                    if (tokens.get(c).equals("int")){
                        while (!tokens.get(c).equals(";")){
                            //this is to insert space between int and var name
                            if(tokens.get(c).equals("int")){
                                sb.append(tokens.get(c));
                                sb.append(" ");
                                c++;
                                continue;
                            }
                            sb.append(tokens.get(c));
                            c++;
                        }
                        sb.append(tokens.get(c)); // append a semicolon
                        type = "init"; // node type: initialization
                        state = 0; // add to root node
                        c += 1; //jump a token ahead for semicolon
                        // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
                        // (otherwise the last line is missing)
                        if (c>=tokens.size()){
                            c--;
                        }
                    }
                    // starts with a variable
                    // could be an assignment
                    else if(tokens.get(c).startsWith("$")){
                        if (tokens.get(c+1).equals("=") || tokens.get(c+1).equals("[")){
                            //clearly an assignment of either a regular var or an array
                            while (!tokens.get(c).equals(";")){
                                System.out.println("adding stuff until semicolon");
                                sb.append(tokens.get(c));
                                c++;
                            }
                            System.out.println("exit variable ");
                            sb.append(tokens.get(c)); // append a semicolon
                            System.out.println("debugstrBuilder " + sb.toString());
                            type = "assign"; // node type: assignment
                            state = 0; // add to root node
                            c += 1; // jump a token ahead for semicolon

                            // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
                            // (otherwise the last line is missing)
                            if (c>=tokens.size()){
                                c--;
                            }
                        }
                        //c++; // debugging placeholder to exit this. MAYBE it can ONLY be VAR in this case
                        break;
                    }
                    // we are entering while loop
                    else if(tokens.get(c).equals("while")){
                        System.out.println("while detected");
                        sb.append(tokens.get(c)); // append while to sb
                        type = "while"; // node type: while
                        state = 2; // go to while loop state
                        c++; // move in tokens
                    }
                    // we are entering if branch
                    else if(tokens.get(c).equals("if")){
                        System.out.println("if detected");
                        sb.append(tokens.get(c)); // append if to sb
                        type = "if"; // node type: if
                        state = 4; // go to if state
                        c++; // move in tokens
                    }
                    // we are processing inner body of a loop or an if condition
                    else if(tokens.get(c).equals("{")){
                        System.out.println("Curly bracket detected");
                        //while (!tokens.get(c).equals("}")){
                            System.out.println("analyzing inner body in between { and }");
                            state = 1; // go to top of state to analyse stuff inside the loop
                            System.out.println("crazy");
                        System.out.println(tokens.get(c));
                        //}
                        c+=1; // jump a token ahead of }
                        System.out.println(tokens.get(c));
                    }
                    else if(tokens.get(c).equals("}")){
                        System.out.println("end of curly bracket detected. Going 2 nodes up.");
                        System.out.println("end of loop body analysis");
                        state = 1;
                        c+=1; // jump ahead
                        placeHolderNode = placeHolderNode.getParent().getParent(); // go 2 nodes up to get to level before while
                        // or if..
                        // MIGHT NEED TO MAKE SOME CHANGES HERE!!!<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                    }
                    else if(tokens.get(c).equals("else")){
                        System.out.println("Else detected. Attaching to current (if) node.");
                        sb.append(tokens.get(c)); // append it to sb
                        type = "else"; // node type: else
                        state = 5; // go to else state
                        c++; // move in tokens

                    }
                    else if(tokens.get(c).equals("read")){
                        while (!tokens.get(c).equals(";")){
                            //this is to insert space between read and var name
                            if(tokens.get(c).equals("read")){
                                sb.append(tokens.get(c));
                                sb.append(" ");
                                c++;
                                continue;
                            }
                            sb.append(tokens.get(c));
                            c++;
                        }
                        sb.append(tokens.get(c)); // append a semicolon
                        type = "read"; // node type: read
                        state = 0; // add to root node
                        c += 1; //jump a token ahead for semicolon
                        // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
                        // (otherwise the last line is missing)
                        if (c>=tokens.size()){
                            c--;
                        }
                    }
                    else if(tokens.get(c).equals("write")){
                        while (!tokens.get(c).equals(";")){
                            if(tokens.get(c).equals("write")){
                                sb.append(tokens.get(c));
                                sb.append(" ");
                                c++;
                                continue;
                            }
                            sb.append(tokens.get(c));
                            c++;
                        }
                        sb.append(tokens.get(c)); // append a semicolon
                        type = "write"; // node type: write
                        state = 0; // add to root node
                        c += 1; //jump a token ahead for semicolon
                        // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
                        // (otherwise the last line is missing)
                        if (c>=tokens.size()){
                            c--;
                        }
                    }
                    else if(tokens.get(c).equals("break")){
                        while (!tokens.get(c).equals(";")){
                            sb.append(tokens.get(c));
                            c++;
                        }
                        sb.append(tokens.get(c)); // append a semicolon
                        type = "break"; // node type: break
                        state = 0; // add to root node
                        c += 1; //jump a token ahead for semicolon
                        // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
                        // (otherwise the last line is missing)
                        if (c>=tokens.size()){
                            c--;
                        }
                    }
                    else if(tokens.get(c).equals("continue")){
                        while (!tokens.get(c).equals(";")){
                            sb.append(tokens.get(c));
                            c++;
                        }
                        sb.append(tokens.get(c)); // append a semicolon
                        type = "continue"; // node type: continue
                        state = 0; // add to root node
                        c += 1; //jump a token ahead for semicolon
                        // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
                        // (otherwise the last line is missing)
                        if (c>=tokens.size()){
                            c--;
                        }
                    }
//                    else if(tokens.get(c).equals(";")){
//                        sb.append(tokens.get(c));
//                        c++;
//                        type = "sc"; // node type: initialization
//                        state = 0; // add to root node
//                        c += 1; //jump a token ahead for semicolon
//                        // below is to ensure we still go to state 0 even if the code is over and while cond. is fulfilled
//                        // (otherwise the last line is missing)
//                        if (c>=tokens.size()){
//                            c--;
//                        }
//                    }
                    //anything else
                    else{
                        System.out.println("state 1 - got something else. STOP PROCESSING");
                        System.out.println("(this is what we got ->)" + tokens.get(c));
                        //c++; // debugging placeholder to finish running the loop
                        c = tokens.size();
                        System.out.println("c = " + c);
                        break;
                    }

                    break;

                case 2: //we encountered WHILE loop, let's deal with this here
                    System.out.println("WHILE: state 2 now");
                    placeHolderNode = placeHolderNode.addChild(new Node(sb.toString(),type)); // add while to current root and save
                    sb = new StringBuilder(); // clear sb
                    type = "condition";
                    //get condition
                    while (!tokens.get(c).equals("{")){
                        if(!tokens.get(c).equals("(") &&
                                !tokens.get(c).equals(")")) {
                            sb.append(tokens.get(c));
                        }
                        c++; // move regardless!
                    }
                    System.out.println("should be before first { now");
                    System.out.println(tokens.get(c));
                    //c++; // we should now be before the first {
                    System.out.println("condition detected: " + sb.toString().replace("$",""));
                    placeHolderNode.addChild(new Node(sb.toString().replace("$",""), type)); // add condition child to while node
                    // note that placeholder is still the WHILE node
                    sb = new StringBuilder();
                    type = "";

                    placeHolderNode = placeHolderNode.addChild(new Node("while_interm","while_interm")); // add intermediate node with while body
                    // note, we now store the INTERM node in placeHolderNode. This is because we want to assign inner loop stuff to it
                    // and treat it as a current ROOT


                    state = 3; // go to state handling intermediate blocks
                    break;
                case 3: // inner part of while loop (i.e. here intermediate block will be joining stuff to itself)
                    System.out.println("case 3 now");
                    System.out.println("maybe we dont need this, going straight to 1");
                    state = 1;
                    break;
                case 4: //we encountered IF keyword, let's deal with this here
                    System.out.println("IF: state 4 now");
                    placeHolderNode = placeHolderNode.addChild(new Node(sb.toString(),type)); // add if to current root and save
                    sb = new StringBuilder(); // clear sb
                    type = "condition";
                    // get condition
                    while (!tokens.get(c).equals("{")){
                        if(!tokens.get(c).equals("(") &&
                                !tokens.get(c).equals(")")) {
                            sb.append(tokens.get(c));
                        }
                        c++; // move regardless!
                    }
                    System.out.println("should be before first { now");
                    System.out.println(tokens.get(c));
                    // we should now be before the first {
                    placeHolderNode.addChild(new Node(sb.toString().replace("$",""), type)); // add condition child to if node
                    // note that placeholder is still the IF node
                    sb = new StringBuilder();
                    type = "";

                    placeHolderNode = placeHolderNode.addChild(new Node("if_interm","if_interm")); // add intermediate node with if body
                    // note, we now store the INTERM node in placeHolderNode. This is because we want to assign inner if stuff to it
                    // and treat it as a current ROOT

                    state = 3; // go to state handling intermediate blocks
                    break;
                case 5: // we encountered ELSE keyword, let's deal with this here
                    System.out.println("ELSE: state 5 now");
                    ArrayList<Node> children = placeHolderNode.getChildren();
                    placeHolderNode = children.get(children.size()-1); // current node is now the IF ...hopefully... (by getting the last child of the cur. node)
                    placeHolderNode = placeHolderNode.addChild(new Node(sb.toString(),type)); // now do as before, add to current root (IF) and save
                    //note, there is no condition here
                    while (!tokens.get(c).equals("{")){
                        //sb.append(tokens.get(c));
                        c++; // simply move through the {
                    }
                    sb = new StringBuilder();
                    type = "";
                    state = 3;
                    break;

            }
        }
        return rootNode;
    }


}
