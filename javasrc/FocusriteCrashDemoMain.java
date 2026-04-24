import java.nio.file.Path;
import java.nio.file.Paths;

public class FocusriteCrashDemoMain {

    // Native methods - only what's absolutely necessary
    public native int loadAsioDriverByName(String driverName);
    public native boolean initAsio();

    public static void main(String[] args) {

        // capture Java version
        String java_version = System.getProperty("java.version");
        System.out.println("JDK:"+java_version);
        System.out.println("JVM Working dir is:"+System.getProperty("user.dir"));
        // Load the native library

        String javaxAsioDllRelativePath2 ="./out_windows_dll/focusrite_minimal.dll";
        String absPath =toAbsolutePath(javaxAsioDllRelativePath2);
        //String absPath =toAbsolutePath("./../out_windows_dll/focusrite_minimal");
        System.load(absPath);

        FocusriteCrashDemoMain demo = new FocusriteCrashDemoMain();

        // Hard-coded Focusrite driver name or RME Raydat
        String driverName = "Focusrite USB ASIO";
        //String driverName = "ASIO Hammerfall DSP"; //check against RME Raydat

        //System.out.println("JVM:Loading driver: " + driverName);
        int result = demo.loadAsioDriverByName(driverName);

        if (result == 0) { // Assuming 0 means success
            System.out.println("JVM:Driver loaded, attempting init...");
            boolean initResult = demo.initAsio();
            System.out.println("JVM:Init result: " + initResult);
        } else {
            System.out.println("JVM:Failed to load driver, result: " + result);
        }
    }
//************************utilities********************
    public static String toAbsolutePath(String relativePath) {
        if (relativePath == null || relativePath.trim().isEmpty()) {
            return null;
        }

        try {
            Path workingDir = Paths.get("").toAbsolutePath();
            Path resolvedPath = workingDir.resolve(relativePath).normalize();
            return resolvedPath.toString().replace('\\', '/');
        } catch (Exception e) {
            System.err.println("JVM:Error resolving path: " + relativePath + " - " + e.getMessage());
            return null;
        }
    }
}