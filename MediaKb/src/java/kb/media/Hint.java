package kb.media;

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-14
 * Time: 下午3:16
 * To change this template use File | Settings | File Templates.
 */

public class Hint {

    private String value = null;
    private int  type = 0;

    Hint() { }

    Hint(String value) {
        this.value = value;
    }

    Hint(String value, int type) {
        this.value = value;
        this.type = type;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

}
