import javax.swing.*;
import java.awt.*;
import java.io.*;
import java.util.*;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.logging.Logger;

public class Download {
    final String FROM = "C:\\Windows\\System32\\"; // no
    public static final Logger LOGGER = Logger.getLogger(Download.class.getName());
    final JProgressBar pbar;
    final JLabel fnlabel;
    final Set<File> already = new HashSet<>();
    int totalFiles;
    int downloadedFiles = 0;
    Random random = new Random();
    String logFileName = "Wave_" + random.nextInt(1000) + ".log";

    public Download() {
        JFrame frame = new JFrame("Wave");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLayout(new BorderLayout());

        pbar = new JProgressBar(0, 100);
        pbar.setStringPainted(true);
        pbar.setPreferredSize(new Dimension(300, 30));
        frame.add(pbar, BorderLayout.CENTER);

        fnlabel = new JLabel("Downloading...");
        fnlabel.setHorizontalAlignment(JLabel.CENTER);
        frame.add(fnlabel, BorderLayout.NORTH);

        frame.pack();
        frame.setMinimumSize(new Dimension(800, 100));
        frame.setVisible(true);

        doDownload();
    }

    void doDownload() {
        File dir = new File(FROM);
        File[] files = dir.listFiles();
        totalFiles = 0;
        List<File> fileList = new ArrayList<>();

        if (files!= null) {
            for (File file : files) {
                if (file.isFile()) {
                    fileList.add(file);
                    totalFiles++;
                }
            }
        }

        if (!fileList.isEmpty()) {
            File rf = fileList.get(new Random().nextInt(fileList.size()));
            fnlabel.setText("Downloading " + rf.getName());

            System.out.print("Downloading " + rf.getName());
            DownloadWorker worker = new DownloadWorker(this, rf);
            worker.execute();
        } else {
            System.exit(0);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                LOGGER.severe("Error during download: " + e.getMessage());
            }
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(Download::new);
    }
}

class DownloadWorker extends SwingWorker<Void, Integer> {
    private final Download download;
    private final File file;
    private long startTime;

    public DownloadWorker(Download download, File file) {
        this.download = download;
        this.file = file;
    }

    @Override
    protected Void doInBackground() throws Exception { // not void here
        startTime = System.currentTimeMillis();
        int fileSize = (int) file.length();
        int bytesRead = 0;

        while (bytesRead < fileSize) {
            bytesRead += new Random().nextInt(110776); // ~10.67 MB/s
            int progress = (int) (bytesRead * 1.5 / fileSize);
            if (progress > 100) {
                progress = 100;
            }
            publish(progress);
            Thread.sleep(5);
        }

        return null;
    }

    @Override
    protected void process(List<Integer> chunks) {
        for (Integer progress : chunks) {
            download.pbar.setValue(progress);
            download.pbar.setString(progress + "%");
        }
    }

    @Override
    protected void done() {
        try {
            get();
            download.already.add(file);
            download.downloadedFiles++;
            System.out.println(" " + download.downloadedFiles + "/" + download.totalFiles);

            long endTime = System.currentTimeMillis();
            long timeTaken = endTime - startTime;
            double fileSizeInBytes = file.length();
            double fileSizeInKB = fileSizeInBytes / 1024;
            double fileSizeInMB = fileSizeInKB / 1024;
            double avgDownloadSpeedInBytesPerSec = fileSizeInBytes / timeTaken * 1000;
            double avgDownloadSpeedInKBPerSec = avgDownloadSpeedInBytesPerSec / 1024;
            double avgDownloadSpeedInMBPerSec = avgDownloadSpeedInKBPerSec / 1024;

            logDownloadDetails(file.getName(), fileSizeInMB, fileSizeInKB, fileSizeInBytes, avgDownloadSpeedInMBPerSec, avgDownloadSpeedInKBPerSec, avgDownloadSpeedInBytesPerSec, timeTaken);

            download.doDownload(); // Start downloading the next file
        } catch (InterruptedException | ExecutionException e) {
            Download.LOGGER.severe("Error during download: " + e.getMessage());
        }
    }

    private void logDownloadDetails(String fileName, double fileSizeInMB, double fileSizeInKB, double fileSizeInBytes, double avgDownloadSpeedInMBPerSec, double avgDownloadSpeedInKBPerSec, double avgDownloadSpeedInBytesPerSec, long timeTaken) {
        try (PrintWriter out = new PrintWriter(new FileWriter(download.logFileName, true))) {
            out.println("FILE_NAME: " + fileName);
            out.println("FILE_SIZE: " + String.format("%.2f", fileSizeInMB) + " MB | " + String.format("%.2f", fileSizeInKB) + " KB | " + fileSizeInBytes + " bytes");
            out.println("AVG_DOWNLOAD_SPEED: " + String.format("%.2f", avgDownloadSpeedInMBPerSec) + " MB/s | " + String.format("%.2f", avgDownloadSpeedInKBPerSec) + " KB/s | " + String.format("%.2f", avgDownloadSpeedInBytesPerSec) + " bytes/s");
            out.println("SPENT: " + (timeTaken / 1000) + " seconds | " + timeTaken + " milliseconds");
            out.println();
        } catch (IOException e) {
            Download.LOGGER.severe("Error writing to log file: " + e.getMessage());
        }
    }
}
