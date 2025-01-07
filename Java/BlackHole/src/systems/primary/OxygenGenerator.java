package systems.primary;

import systems.PowerSupplier;
import systems.SpacecraftData;

public class OxygenGenerator implements Runnable {
    private PowerSupplier supplier;
    private SpacecraftData spacecraftData;
    private String name;
    private int powerNeeded = 1;

    public OxygenGenerator(String name ,PowerSupplier supplier, SpacecraftData spacecraftData) {
        this.name = name;
        this.supplier = supplier;
        this.spacecraftData = spacecraftData;
    }

    @Override
    public void run() {
        boolean checking = true;
        while (!Thread.interrupted() && checking) {
            try {
                Thread.sleep(1000);
                boolean res = supplier.consumePower(powerNeeded);
                if(res)
                {
                    System.out.println("[" + name + "]: Oxygen generator is running, consuming " + powerNeeded + " units of power.");
                }
                else
                {
                    Thread.sleep(2000);
                    res = supplier.consumePower(powerNeeded);
                    if(res)
                    {
                        System.out.println("[" + name + "]: Oxygen generator is running, consuming " + powerNeeded + " units of power.");
                    }
                    else
                    {
                        System.out.println("[" + name + "]: the crew has died (Oxygen generator is failed due to insufficient power)!");
                        spacecraftData.isLifeOnBoard = false;
                        checking = false;
                    }
                }
            }
            catch (Exception e) {
                System.out.println("[" + name + "]: Re-running oxygen checks...");
                if(e instanceof InterruptedException)
                {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
}
