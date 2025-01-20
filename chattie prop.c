/*
Hier ist ein strukturiertes Vorgehen, um das Projekt erfolgreich zu meistern:

---

### **1. Gemeinsames Verständnis schaffen**

Bevor ihr loslegt, solltet ihr sicherstellen, dass ihr beide ein klares Verständnis der Anforderungen und Ziele habt.
- **Dokumentation der Anforderungen**: Erstellt gemeinsam ein Dokument, in dem alle Features und Spezifikationen der Shell zusammengefasst sind.
- **Klärung von Unklarheiten**: Besprecht, was bestimmte Anforderungen bedeuten, z. B. wie der Umgang mit Zeichen wie `$` oder `"` genau funktionieren soll.

---

### **2. Aufgabenaufteilung und Rollen**

Da ihr noch nie zusammengearbeitet habt, hilft es, klare Verantwortlichkeiten zu definieren:
- **Rollen basierend auf Stärken**: Überlegt, wer von euch sich wohler mit bestimmten Bereichen fühlt (z. B. Systemaufrufe, String-Parsing, Signal-Handling).
- **Komponentenorientierte Aufteilung**: Teilt die Aufgaben nach logischen Modulen der Shell auf. Beispiele:
  - **Input-Handling**: Verarbeitung von Benutzereingaben und Parsing.
  - **Command-Execution**: Ausführung von Kommandos (z. B. Built-ins, externe Programme, Pipes).
  - **Redirections**: Umgang mit `<`, `>`, `<<`, `>>`.
  - **Signal-Handling**: Implementierung von `Ctrl-C`, `Ctrl-D`, `Ctrl-\`.
  - **Environment-Handling**: Verarbeitung von Variablen und `$?`.
  - **Testing**: Erstellung von Tests, um sicherzustellen, dass alles wie erwartet funktioniert.

**Vorschlag für die Startaufteilung:**

- Teammate 1: Command-Parsing und Built-ins (z. B. `cd`, `export`, `unset`).
- Teammate 2: Pipes, Redirections und Signal-Handling.

Ihr könnt später an den Modulen des anderen arbeiten, aber eine klare Aufteilung erleichtert den Einstieg.

---

### **3. Technische Planung**

Erstellt eine Übersicht über die Architektur der Shell:
- **Main Loop**: Der Haupt-Loop der Shell, der Eingaben einliest und verarbeitet.
- **Parser**: Zerlegt die Eingabe in Befehle und Argumente, unter Beachtung von Quotes und Meta-Zeichen.
- **Executor**: Führt die Befehle aus (z. B. Aufruf von Built-ins oder externe Programme).
- **Redirection & Pipes**: Behandelt I/O-Redirection und Verbindungen zwischen Prozessen.
- **Signal Handling**: Sorgt für sauberes Verhalten bei `Ctrl-C`, `Ctrl-D` usw.

---

### **4. Versionierung und Tools**

Verwendet Tools, die euch die Zusammenarbeit erleichtern:
- **Git**: Legt ein gemeinsames Repository an und arbeitet in Feature-Branches, um Konflikte zu minimieren.
  - Beispiel-Branches: `parsing`, `redirection`, `pipes`.
- **Code Reviews**: Führt regelmäßige Code Reviews durch, um die Qualität zu sichern und voneinander zu lernen.
- **Task-Tracking**: Nutzt ein Tool wie Trello, GitHub Issues oder Notion, um Aufgaben zu verfolgen.

---

### **5. Entwicklungsworkflow**

1. **Erste Schritte**: Arbeitet gemeinsam an einem Grundgerüst:
   - Implementiert eine minimal lauffähige Shell, die Eingaben liest und ein einfaches Kommando (z. B. `echo`) ausführt.
2. **Iterative Entwicklung**: Baut das Projekt schrittweise aus:
   - Startet mit Built-ins (`cd`, `pwd`) und fügt dann komplexere Funktionen wie Pipes und Redirections hinzu.
3. **Code Reviews und Tests**:
   - Schreibt Unit-Tests für den Parser, die Built-ins und die Kommandoverarbeitung.
   - Testet Edge-Cases (z. B. leerer Input, Verschachtelung von Quotes).

---

### **6. Kommunikation**

- **Regelmäßige Meetings**: Plant tägliche oder wöchentliche Sync-Meetings, um den Fortschritt zu besprechen.
- **Pair Programming**: Arbeitet bei schwierigeren Aufgaben zusammen am selben Code (z. B. via Live-Share).
- **Feedback-Kultur**: Diskutiert offen, was gut läuft und wo ihr Schwierigkeiten habt.

---

### **7. Beispiele für Aufgabenplanung**

#### **Woche 1**: 
- Parser-Grundgerüst: Erkennen von Kommandos, Argumenten und Quotes.
- Implementierung der ersten Built-ins (`echo`, `cd`).

#### **Woche 2**:
- Hinzufügen von Pipes und einfacher I/O-Redirection (`<`, `>`).
- Signal-Handling für `Ctrl-C`, `Ctrl-D`.

#### **Woche 3**:
- Erweiterung des Parsers um komplexe Redirections (`>>`, `<<`) und das Handling von `$` und `$?`.
- Testing der bisherigen Funktionen.

#### **Woche 4**:
- Integration und Debugging.
- Dokumentation und Feinschliff.

---

### **8. Tipps für die Zusammenarbeit**

- **Teilt Wissen aktiv**: Erklärt einander, wie ihr Probleme gelöst habt.
- **Respektiert Deadlines**: Verabredet Deadlines für Module und testet eure Arbeit frühzeitig.
- **Bleibt flexibel**: Falls ein Bereich zu schwierig ist, könnt ihr die Verantwortlichkeiten neu verteilen.

Mit diesem Ansatz könnt ihr das Projekt sowohl technisch als auch kollaborativ erfolgreich umsetzen. Viel Erfolg!

*/