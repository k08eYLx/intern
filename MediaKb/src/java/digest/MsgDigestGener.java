package digest;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;

import java.io.*;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public final class MsgDigestGener {

    private static final int BUF_SIZE = 8192;
    private static final String DEFAULT_CHARSET = "UTF-8";

    private static final String MD5  = "MD5";
    private static final String SHA1 = "SHA1";    // "SHA-1"

	private static final char[] HEX_DIGITS = { '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    private static byte[] getBytes(String str) {
        return getBytes(str, DEFAULT_CHARSET);
    }

    private static byte[] getBytes(String str, String charset) {
        byte[] bytes = new byte[0];
        try {
            bytes = str.getBytes(charset);
        }
        catch (UnsupportedEncodingException uee) {
            uee.printStackTrace();
        }
        return bytes;
    }

    public static String md5Encode(String str) {
        if (str == null) return null;
        else return md5Encode(getBytes(str));
    }

    public static String md5Encode(byte[] bytes) {
        return encode(MD5, bytes);
    }

    public static String sha1Encode(File file) {
        if (!(file.exists())) return null;
        FileInputStream fileInputStream = null;
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA1");
            fileInputStream = new FileInputStream(file);
            byte[] buffer = new byte[BUF_SIZE];
            int length = -1;
            while ((length = fileInputStream.read(buffer)) != -1) {
                digest.update(buffer, 0, length);
            }
            return new String(Hex.encodeHex(digest.digest()));
        }
        catch (FileNotFoundException fnfe) {
            fnfe.printStackTrace();
        }
        catch (IOException ioe) {
            ioe.printStackTrace();
        }
        catch (NoSuchAlgorithmException nsae) {
            nsae.printStackTrace();
        }
        finally {
            try {
                if (fileInputStream != null) {
                    fileInputStream.close();
                }
            }
            catch (IOException ioe) {
                ioe.printStackTrace();
            }
        }
        return null;
    }

    /**
     * @param str    file name or string to be encoded
     * @return
     */
    public static String sha1Encode(String str) {
        if (str == null) return null;
        else {
            File file = new File(str);
            if (file.isFile()) return sha1Encode(file);
            else return sha1Encode(getBytes(str));
        }
    }

    public static String sha1Encode(byte[] bytes) {
        return encode(SHA1, bytes);
    }

    public static String encode(String algorithm, String str) {
        if (str == null) return null;
        else return encode(algorithm, getBytes(str));
    }

	public static String encode(String algorithm, byte[] bytes) {
		try {
			MessageDigest messageDigest = MessageDigest.getInstance(algorithm);
			messageDigest.update(bytes);
			return getFormattedText(messageDigest.digest());
		}
		catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

	private static String getFormattedText(byte[] bytes) {
		int len = bytes.length;
		StringBuilder buf = new StringBuilder(len * 2);
		// 把密文转换成十六进制的字符串形式
		for (int j = 0; j < len; j++) {
			buf.append(HEX_DIGITS[(bytes[j] >> 4) & 0x0f]);
			buf.append(HEX_DIGITS[bytes[j] & 0x0f]);
		}
		return buf.toString();
	}

    private static String encodeToHex(byte[] bytes) {
        BigInteger bigInt = new BigInteger(1, bytes);
		String.format("%1$032x", bigInt);
		String hashtext = bigInt.toString(16);
		// Now we need to zero pad it if you actually want the full 32 chars.
		while (hashtext.length() < 32 ) {
		  	hashtext = "0" + hashtext;
		}
        return hashtext;
    }

    private static String toHex(byte[] bytes) {
        return (new String(Hex.encodeHex(bytes)));
    }

    private static String encodeToString(byte[] bytes) {
        System.out.println(new String(Hex.encodeHex(bytes)));
        StringBuffer strBuf = new StringBuffer();
        for (int i = 0; i < bytes.length; i++) {
            int b = 0xFF & bytes[i];
            String bStr = Integer.toHexString(b);
            if (bStr.length() == 1) strBuf.append("0");
            strBuf.append(bStr);
        }
        return strBuf.toString();
    }

    public String bytes2Hex(byte[] bts) {
		String des = "";
		String tmp = null;
		for (int i = 0; i < bts.length; i++) {
			tmp = (Integer.toHexString(bts[i] & 0xFF));
			if (tmp.length() == 1) des += "0";
			des += tmp;
		}
		return des;
	}

    public static String shaHex(String pathname) {
        String shaHex = null;

        File file = new File(pathname);
        if (file.exists()) {
            try {
                shaHex = DigestUtils.shaHex(new FileInputStream(file));
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }

        return shaHex;
    }

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("111111 MD5 :\t"
				+ MsgDigestGener.md5Encode("111111"));
        System.out.println("111111 SHA1 :\t"
        		+ MsgDigestGener.sha1Encode("111111"));
        System.out.println("111111 SHA-256 :\t"
        		+ MsgDigestGener.encode("SHA-256", "111111"));

        System.out.println(MsgDigestGener.sha1Encode(new File("etc/algorithm.txt")));
        System.out.println(MsgDigestGener.sha1Encode(new File("etc/algorithmCopy.txt")));

        long start = System.currentTimeMillis();
        System.out.println(MsgDigestGener.shaHex("etc/demo.avi"));
        long finish = System.currentTimeMillis();
        System.out.println(finish - start);
    }

}
