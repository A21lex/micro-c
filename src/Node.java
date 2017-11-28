/**
 * Created by aleksandrs on 11/28/17.
 */

import java.util.ArrayList;

/**
 * Node data structure
 */
public class Node {
    public String nodeData = null;
    public String nodeType = null; // type of a node (root, init., ass., interm., while, if, etc.)
    public Node parent = null; // top parent is always root
    public ArrayList<Node> children = new ArrayList<>();

    public Node(String nodeData){
        this.nodeData = nodeData;
    }

    public Node(String nodeData, String nodeType){
        this.nodeData = nodeData;
        this.nodeType = nodeType;
    }

    //add a child node
    public Node addChild(Node child){
        child.setParent(this);
        this.children.add(child);
        return child;
    }

    public String getNodeData(){
        return nodeData;
    }
    public void setNodeData(String nodeData){
        this.nodeData = nodeData;
    }

    public Node getParent(){
        return parent;
    }

    public void setParent(Node parent){
        this.parent = parent;
    }

    public ArrayList<Node> getChildren(){
        return children;
    }

    public void setNodeType(String nodeType){
        this.nodeType = nodeType;
    }

    public String getNodeType() {
        return nodeType;
    }

    public static Node createTree(){
        Node root = new Node("root","root"); // root node
        Node child1 = root.addChild(new Node("child1","ass"));
        Node child2 = root.addChild(new Node("child2","ass"));
        Node child3 = root.addChild(new Node("child3","while"));
        Node child31 = child3.addChild(new Node("child31","interm"));
        return root; // root is, in essence, a tree
    }

    public static void printTree(Node node, String appender){
        System.out.println(appender + node.getNodeData());
        node.getChildren().forEach(child -> printTree(child, appender + appender));
    }

    public static void main(String[] args) {
        Node tree = createTree();
        printTree(tree, "-");
        ArrayList<Node> babies = tree.getChildren();
        for (Node baby: babies){
            System.out.println(baby.getNodeData() + ":" + baby.getNodeType());
        }

    }
}
