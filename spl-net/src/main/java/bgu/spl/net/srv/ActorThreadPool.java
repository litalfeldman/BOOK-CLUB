package bgu.spl.net.srv;

import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.WeakHashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class ActorThreadPool {

    private final Map<Object, Queue<Runnable>> map_todo;
    private final ReadWriteLock actsRWLock;
    private final Set<Object> set_playerz;
    private final ExecutorService stupid_ThreadPool;

    public ActorThreadPool(int threads) {
        this.stupid_ThreadPool = Executors.newFixedThreadPool(threads);
        map_todo = new WeakHashMap<>();
        set_playerz = ConcurrentHashMap.newKeySet();
        actsRWLock = new ReentrantReadWriteLock();
    }

    public void submit(Object handler, Runnable mission) {
        synchronized (handler) {
            if (!set_playerz.contains(handler)) {
                set_playerz.add(handler);
                execute(mission, handler);
            } else {
                pendingRunnablesOf(handler).add(mission);
            }
        }
    }

    public void shutdown() {
        stupid_ThreadPool.shutdownNow();
    }

    private Queue<Runnable> pendingRunnablesOf(Object handler) {

        actsRWLock.readLock().lock();
        Queue<Runnable> pendingRunnables = map_todo.get(handler);
        actsRWLock.readLock().unlock();

        if (pendingRunnables == null) {
            actsRWLock.writeLock().lock();
            map_todo.put(handler, pendingRunnables = new LinkedList<>());
            actsRWLock.writeLock().unlock();
        }
        return pendingRunnables;
    }

    private void execute(Runnable mission, Object handler) {
        stupid_ThreadPool.execute(() -> {
            try {
                mission.run();
            } finally {
                complete(handler);
            }
        });
    }

    private void complete(Object handler) {
        synchronized (handler) {
            Queue<Runnable> pending = pendingRunnablesOf(handler);
            if (pending.isEmpty()) {
                set_playerz.remove(handler);
            } else {
                execute(pending.poll(), handler);
            }
        }
    }

}
