# ДЗ 9. Паттерны и UML

## Состояние, снимок, прототип

- **State.** Банкомат делегирует обработку событий состоянию `Idle`, `CheckingPin`, `ChoosingAmount` или `Dispensing`. Карта, верный PIN, отмена и подтверждение суммы переключают состояния. Отмена возвращает карту в состояниях до выдачи. Ошибка выдачи передаётся обработчику транзакции, который отменяет списание.
- **Memento.** Перед редактированием графический редактор сохраняет неизменяемый снимок документа в стеке Undo и очищает Redo. Undo переносит текущий снимок в Redo и применяет верхний элемент Undo. Redo выполняет обратную операцию. Снимок содержит данные документа и создаётся самим документом.
- **Prototype.** Таблица прототипов хранит настроенные образцы врагов. Фабрика вызывает виртуальный `Clone` у образца нужного типа, задаёт позицию экземпляру и добавляет его на карту. Изменяемые данные вроде здоровья копируются для экземпляра.

```mermaid
classDiagram
    ATM --> ATMState
    ATMState <|.. Idle
    ATMState <|.. CheckingPin
    ATMState <|.. ChoosingAmount
    ATMState <|.. Dispensing
    Document ..> DocumentSnapshot : создает
    EditorHistory o-- DocumentSnapshot
    EnemySpawner --> EnemyPrototype
    EnemyPrototype <|.. ScoutPrototype
    EnemyPrototype <|.. GuardianPrototype
    EnemyPrototype : Clone()
```

## Arkanoid

```mermaid
classDiagram
    Game o-- IGameState
    IGameState <|.. PlayingState
    GameObject <|-- Ball
    GameObject <|-- Platform
    GameObject <|-- Brick
    GameObject <|-- Bonus
    Brick <|-- GlassBrick
    PlayingState o-- Ball
    PlayingState o-- Platform
    PlayingState o-- Brick
    PlayingState o-- Bonus
    PlayingState *-- ScoreSystem
    PlayingState *-- PlayingMemento
    PlayingMemento *-- BrickSnapshot
    ScoreSystem o-- IScoreObserver
    IScoreObserver <|.. PlayingState
    BonusFactory ..> Bonus : создает
    Bonus *-- IBonusEffect
    IBonusEffect <|.. ExpandPlatformEffect
    IBonusEffect <|.. SlowBallEffect
    IBonusEffect <|.. DoubleScoreEffect
```

| Событие | Участники | Результат |
| --- | --- | --- |
| Начисление | `PlayingState → ScoreSystem → IScoreObserver` | Изменяется счёт, наблюдатель обновляет текст интерфейса. |
| Снимок | `PlayingState → PlayingMemento → BrickSnapshot` | Сохраняются очки и состояние кирпичей, которые можно восстановить после потери жизни. |
| Выпадение | `PlayingState → BonusFactory → Bonus` | При разрушении кирпича вероятность появления бонуса 10%, длительность эффекта от 5 до 10 секунд. |
| Широкая платформа | `Bonus → ExpandPlatformEffect → Platform` | Размер платформы увеличивается и затем возвращается к исходному. |
| Медленный мяч | `Bonus → SlowBallEffect → Ball` | Скорость мяча временно снижается. |
| Удвоение очков | `Bonus → DoubleScoreEffect → BonusRuntimeContext` | Временно увеличивается множитель набранных очков. |

## Начисление очков и восстановление снимка

```mermaid
sequenceDiagram
    participant G as PlayingState
    participant S as ScoreSystem
    participant M as PlayingMemento
    G->>S: AddPoints за разрушенный кирпич
    S-->>G: Уведомление наблюдателя IScoreObserver
    G->>M: Сохранить очки и список BrickSnapshot
    Note over G,M: Мяч упал, есть оставшаяся жизнь
    M-->>G: Последние очки и снимки кирпичей
    G->>S: Восстановить счет
    S-->>G: Обновить текст счета
```

## Бонус широкой платформы

```mermaid
sequenceDiagram
    participant G as PlayingState
    participant B as Bonus
    participant E as ExpandPlatformEffect
    participant P as Platform
    G->>B: Apply(context)
    B->>E: Apply(context)
    E->>P: Расширить платформу
    Note over G,P: По истечении времени действия
    G->>B: Revert(context)
    B->>E: Revert(context)
    E->>P: Восстановить ширину
```

## Бонус медленного мяча

```mermaid
sequenceDiagram
    participant G as PlayingState
    participant B as Bonus
    participant E as SlowBallEffect
    participant M as Ball
    G->>B: Apply(context)
    B->>E: Apply(context)
    E->>M: Уменьшить скорость
    Note over G,M: По истечении времени действия
    G->>B: Revert(context)
    B->>E: Revert(context)
    E->>M: Восстановить скорость
```

## Бонус удвоения очков

```mermaid
sequenceDiagram
    participant G as PlayingState
    participant B as Bonus
    participant E as DoubleScoreEffect
    participant C as BonusRuntimeContext
    G->>B: Apply(context)
    B->>E: Apply(context)
    E->>C: Установить множитель 2
    Note over G,C: По истечении времени действия
    G->>B: Revert(context)
    B->>E: Revert(context)
    E->>C: Восстановить множитель 1
```
