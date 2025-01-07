package systems.secondary;

import systems.PowerSupplier;
import systems.SpacecraftData;

public class CommunicationHandler implements Runnable {
    private final PowerSupplier supplier;
    private final SpacecraftData spacecraftData;
    private final String name;
    private final int powerNeeded = 1;
    private SecondarySystemState secondarySystemState = SecondarySystemState.HEALTHY;

    public CommunicationHandler(String name, PowerSupplier supplier, SpacecraftData spacecraftData)
    {
        this.supplier = supplier;
        this.spacecraftData = spacecraftData;
        this.name = name;
    }

    @Override
    public void run()
    {
        boolean checking = true;
        while (!Thread.currentThread().isInterrupted() && checking)
        {
            try {
                Thread.sleep(2000);
                if(secondarySystemState != SecondarySystemState.CRITICAL)
                {
                    boolean res = supplier.consumePower(powerNeeded);
                    if (res)
                    {
                        System.out.println("[" + name + "]: CommunicationHandler is running, consuming " + powerNeeded + " units of power.");
                        secondarySystemState = SecondarySystemState.HEALTHY;
                    } else
                    {
                        Thread.sleep(2000);
                        res = supplier.consumePower(powerNeeded);
                        if (res)
                        {
                            System.out.println("[" + name + "]: CommunicationHandler is running, consuming " + powerNeeded + " units of power.");
                            secondarySystemState = SecondarySystemState.HEALTHY;
                        } else
                        {
                            if (secondarySystemState == SecondarySystemState.HEALTHY)
                            {
                                secondarySystemState = SecondarySystemState.UNHEALTHY;
                                System.out.println("[" + name + "]: Not enough power for CommunicationHandler. Switching off...");

                            } else
                            {
                                secondarySystemState = SecondarySystemState.CRITICAL;
                            }
                        }
                    }
                }
                else
                {
                    System.out.println("[" + name + "]: the crew has died (A malfunction occurred in the CommunicationHandler.)!");
                    spacecraftData.isLifeOnBoard = false;
                    secondarySystemState = SecondarySystemState.MALFUNCTIONED;
                }
                checking = spacecraftData.isLifeOnBoard;
            }
            catch (Exception e)
            {
                if(e instanceof InterruptedException)
                {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
}
