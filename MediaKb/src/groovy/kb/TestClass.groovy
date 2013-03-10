package kb

import digest.MsgDigestGener
import groovy.json.JsonOutput
import kb.media.Hint
import kb.media.MediaInfo
import kb.media.MediaKbService
import matcher.Preprocessor
import matcher.Translator
import utils.Tracer

/**
 * Created with IntelliJ IDEA.
 * User: YangLX
 * Date: 13-1-14
 * Time: 上午11:00
 * To change this template use File | Settings | File Templates.
 */

class TestClass {

    private static MediaKbService kbMgr = new MediaKbService()

    private static void getByJson() {
        // 一次请求中的各个子hint之间应该是与的关系
        String jsnHint =  '{"hints":[{"hint":{"value":"爱×无限大","type":1}},{"hint":{"value":"元若蓝","type":2}}]}'
        List<MediaInfo> infos = kbMgr.getMediaInfosByJson(jsnHint)
        infos.each { println(it) }
    }

    private static void getByHint() {
        String value = "爱冒险"
        Hint hint = new Hint(value, 1)
        List<MediaInfo> infos = kbMgr.getMediaInfosByHint(hint)
        infos.each { println(it) }
    }

    private static void getByHints() {
        String value = "爱冒险"
        Hint hint = new Hint(value)
        List<Hint> hints = new ArrayList<Hint>()
        hints.add(hint)
        List<MediaInfo> infos = kbMgr.getMediaInfosByHints(hints)
        infos.each { println(it) }
    }

    private static void getBySha1() {
        String sha1 = "70cf53ee85a922abc9be5310a04b2d1f2cbaac8c"
        String fileName = "汪峰-北京北京-华语"
        MediaInfo info = kbMgr.getMediaInfoBySHA1(sha1)
        if (info == null) {
            List<MediaInfo> infos = kbMgr.getMediaInfosByHint(fileName)
            infos.each { println(it) }
        }
        else {
            println(info)
        }
    }

    private static void getBySha1AndName() {
        String fileName = "etc/汪峰-北京北京-华语.mv"
        String sha1 = MsgDigestGener.sha1Encode(fileName)
        String json = kbMgr.getMediaInfosBySha1AndName(sha1, fileName)
        println(JsonOutput.prettyPrint(json))
    }

    private static void getByName() {
        /*
         * "[英文】-Timbaland&Apologize"
         * "Imagine Dragons-It's Time"
         * "[英文]-Jay-Z-Numb/Encore (Explicit Version)"  // 9152812
         */
        String hint = "卓义峰－我期待(欣赏版)(中国好声音)"
        List<MediaInfo> infos = kbMgr.getMediaInfosByHint(hint)
        infos.each { println(it) }
    }

    private static void namesTest(String fileName) {
        Translator translator = new Translator("etc/map.translate");
        FileReader reader = new FileReader(fileName)
        LineNumberReader lineReader = new LineNumberReader(reader);
        int count = 0
        int notCount = 0
        String line = null
        while ((line = lineReader.readLine()) != null) {
            line = line.trim()
            if (!line.isEmpty()) {
                Tracer.log(++count + " ===> ${line}\r\n")
                line = translator.translate(line)
                //line = Preprocessor.rmAdditionalInfo(line)
                List<MediaInfo> infos = kbMgr.getMediaInfosByHint(line)
                if (infos.size() == 0 ) ++notCount
                infos.each { Tracer.log("" + it) }
                Tracer.log("\r\n\r\n\r\n")
            }
        }
        Tracer.log("\n\n\n${notCount}")
    }

    public static void main(String[] args) {
        long start = System.currentTimeMillis()
        //getByName()
        //getByHint()
        //getBySha1AndName()
        namesTest("etc/数据爬取问题.txt")    // "etc/names-test.txt"
        long finish = System.currentTimeMillis()
        println(finish - start)
    }

}
