package systems;

import java.util.concurrent.*;

import systems.Spacecraft;

public class SpaceFleet extends Thread {
    ExecutorService fleet = Executors.newFixedThreadPool(10);
    BlockingQueue<Future<SpacecraftData>> spacecrafts = new LinkedBlockingQueue<Future<SpacecraftData>>();

    public void run() {
        try
        {
            for (int i = 0; i < 10; i++) {
                Spacecraft s = new Spacecraft("Edminton-" + i);
                Future<SpacecraftData> f = fleet.submit(() -> {
                    s.run();
                    return s.status();
                });
                spacecrafts.add(f);
            }
            sleep(3000);
            fleet.shutdownNow();


            System.out.println(spacecrafts.poll().get().isLifeOnBoard);

        } catch (Exception e) {
            System.out.println("SpaceFleet thread interrupted");
        }
    }
}
