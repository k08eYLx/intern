package codec.trans;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.util.List;

/**
 * 
 * Java可以直接调用Linux命令，形式如下：
 * 	Runtime.getRuntime().exec(command)
 * （注意：执行Runtime.getRuntime().exec（command）将产生一个本地的进程，
 * 并返回的是一个Process类的实例，该实例可用于控制进程或取得进程的相关信息。）
 * 举例：
 * 	运行ls，top命令可以这样：
 * 		Runtime.getRuntime().exec("ls");
 * 但是这样执行时没有任何输出，原因：
 * 	由于调用Runtime.exec方法所创建的子进程没有自己的终端或控制台，因此该子进程的标准IO(如stdin，stdout，stderr)都重定向给了它的父进程。
 * 	用户需要通过Process.getInputStream()，Process.getOutputStream()，Process.getErrorStream()这些方法
 * 	获取到的stream来向子进程输入数据或获取子进程的输出。
 * 
 * @author xiangzi
 *
 */
public class CmdExecuter {

    public static InputStream getInputStream(String cmd) {
        InputStream stream = null;
        try {
            Process process = Runtime.getRuntime().exec(cmd);
            stream = process.getInputStream();
        }
        catch (IOException e) {
            System.err.println("IOException " + e.getMessage());
        }
        return stream;
    }

    public static LineNumberReader getLineReader(String cmd) {
        InputStreamReader inputStreamReader = new InputStreamReader(getInputStream(cmd));
        return (new LineNumberReader(inputStreamReader));
    }

	public static void execute(String cmd) {	
		try {
			LineNumberReader  reader = getLineReader(cmd);
			String line = null;
			while ((line = reader.readLine()) != null) {
				System.out.println(line);
			}
		}
		catch (IOException e) {
			System.err.println("IOException " + e.getMessage());
		}
	}
	
	public static void pbExecute(String cmd) {
		ProcessBuilder builder = new ProcessBuilder();
		builder.command(cmd);
		try {
			builder.start();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static void pbExecute(List<String> cmds) {
		ProcessBuilder builder = new ProcessBuilder();
		builder.command(cmds);
		try {
			builder.start();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		CmdExecuter.execute("dir");
	}

}
