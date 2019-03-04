import java.io.*;
import java.net.*;

public class SqTCPServ {

    private static final int PORT = 5000;

    private BufferedWriter w;
    private BufferedReader r;
    private Socket ns;
    private ServerSocket s;

    public SqTCPServ() {
        try {
            System.out.println("listening on port " + PORT);
            s = new ServerSocket(PORT);
            ns = s.accept();
            s.close();
            System.out.println("connection accepted");
            r = new BufferedReader(new InputStreamReader(ns.getInputStream()));
            w = new BufferedWriter(new OutputStreamWriter(ns.getOutputStream()));
            char[] cbuf = new char[100];
            String v = new String(cbuf, 0, r.read(cbuf, 0, cbuf.length));
            System.out.println("got: " + v);
            int i = Integer.parseInt(v);
            i = i * i;
            w.write(String.valueOf(i));
            w.flush();
            System.out.println("sent: " + i);
            ns.close();
        } catch (IOException e) {
            System.err.println("i/o error: " + e.getMessage());
        } catch (NumberFormatException e) {
            System.err.println("not an integer");
        } 
    }

    public static void main(String[] args) {
        new SqTCPServ();
    }
}
