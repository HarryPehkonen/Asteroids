#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include <SFML/Graphics.hpp>
#include "DebugUtils.hpp"

template<typename T>
class GameObjectManager {
public:
    void update(float deltaTime) {
        GOT_HERE();
        // Move pending objects to current
        for (auto& obj : pending) {
            if (obj) {
                current.push_back(std::move(obj));
            }
        }
        pending.clear();

        // Move valid objects to next buffer and update them
        next.clear();
        for (auto& obj : current) {
            if (!obj) continue;
            
            obj->update(deltaTime);
            next.push_back(std::move(obj));
        }

        // Swap buffers
        current.swap(next);
        next.clear();

        GOT_HERE();
    }
    
    void draw(sf::RenderWindow& window) const {
        for (const auto& obj : current) {
            if (obj) {
                obj->draw(window);
            }
        }
    }
    
    void spawn(std::unique_ptr<T> obj) {
        if (obj) {
            pending.push_back(std::move(obj));
        }
    }
    
    void clear() {
        current.clear();
        next.clear();
        pending.clear();
    }
    
    size_t count() const {
        return current.size() + pending.size();
    }
    
    bool empty() const {
        return current.empty() && pending.empty();
    }

    void removeIf(std::function<bool(const T&)> predicate) {
        auto removeFromBuffer = [&predicate](std::vector<std::unique_ptr<T>>& buffer) {
            auto it = std::remove_if(buffer.begin(), buffer.end(),
                [&predicate](const std::unique_ptr<T>& obj) {
                    return obj && predicate(*obj);
                });
            buffer.erase(it, buffer.end());
        };

        size_t currentSizeBefore = current.size();
        size_t pendingSizeBefore = pending.size();
        removeFromBuffer(current);
        removeFromBuffer(pending);
        size_t currentSizeAfter = current.size();
        size_t pendingSizeAfter = pending.size();
        LOG_VALUE("Current size before", currentSizeBefore);
        LOG_VALUE("Current size after", currentSizeAfter);
        LOG_VALUE("Pending size before", pendingSizeBefore);
        LOG_VALUE("Pending size after", pendingSizeAfter);
    }
    
    const std::vector<std::unique_ptr<T>>& getObjects() const {
        LOG("Getting objects");
        LOG_VALUE("Current size", current.size());
        LOG_VALUE("Pending size", pending.size());
        LOG_VALUE("Next size", next.size());
        return current;
    }

private:
    std::vector<std::unique_ptr<T>> current;
    std::vector<std::unique_ptr<T>> next;
    std::vector<std::unique_ptr<T>> pending;
};
