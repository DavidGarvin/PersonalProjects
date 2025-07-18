package com.dlg.FlashCard;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.TextField;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.io.File;

public class Main extends Application {
    // Change the filepath to be relative to the class location
    private final String flashcardsFile = "src/main/resources/flashcards.csv";

    private List<Flashcard> flashcards;
    private int currentIndex = 0;
    private boolean showingBase = true, showingIchPlural = true, showingDu = true,
                    showingErSieEs = true, showingWir = true, showingIhr = true, showingSieSie = true, showingPerfect = true;
    private Label flashcardLabel, baseLabel, ichPluralLabel, duLabel, erSieEsLabel, wirLabel, ihrLabel,
                    sieSieLabel, perfectLabel;
    private Button flipIchPluralButton;
    private Button flipDuButton;
    private Button flipErSieEsButton;
    private Button flipWirButton;
    private Button flipIhrButton;
    private Button flipSieSieButton;
    private Button flipPerfectButton;

    // Modify loadFlashcardsFromCSV to create the file if it doesn't exist
    private List<Flashcard> loadFlashcardsFromCSV(String filePath) {
        List<Flashcard> list = new ArrayList<>();
        
        // Create file if it doesn't exist
        try {
            File file = new File(filePath);
            if (!file.exists()) {
                file.createNewFile();
                // Write header
                try (BufferedWriter writer = new BufferedWriter(new FileWriter(file))) {
                    writer.write("type,question,base,ichPlural,du,erSieEs,wir,ihr,sieSie,perfect");
                    writer.newLine();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        // Rest of the method remains the same
        try(BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line;
            boolean firstLine = true;
            while((line = br.readLine()) != null) {
                if (firstLine) { //skip header
                    firstLine = false;
                    continue;
                }

                String[] parts = line.split(",", 0);
                if(parts.length == 4) {
                    list.add(new Flashcard(parts[0].trim(), parts[1].trim(), parts[2].trim(), parts[3].trim()));
                }
                else if(parts.length == 10) {
                    list.add(new Flashcard(parts[0].trim(), parts[1].trim(), parts[2].trim(), parts[3].trim(),
                            parts[4].trim(), parts[5].trim(), parts[6].trim(), parts[7].trim(), parts[8].trim(), parts[9].trim()));
                }
            }
        } catch(IOException e) {
            e.printStackTrace();
        }

        Collections.shuffle(list);
        
        return list;
    }

    public void addCard(String filePath, String input) {
        try(BufferedWriter writer = new BufferedWriter(new FileWriter(filePath, true))) {
            writer.write(input);
            writer.newLine();
        } catch(IOException e) {
            e.printStackTrace();
        }

        flashcards.clear();
        flashcards = loadFlashcardsFromCSV(flashcardsFile);
        currentIndex = 0;
        updateCard();
    }

    @Override
    public void start(Stage stage) {
        flashcards = loadFlashcardsFromCSV(flashcardsFile);

        flashcardLabel = new Label();
        flashcardLabel.setStyle("-fx-font-size: 22px; -fx-font-weight: bold; -fx-alignment: center;");

        baseLabel = new Label(); baseLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); baseLabel.setWrapText(true);
        ichPluralLabel = new Label(); ichPluralLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); ichPluralLabel.setWrapText(true);
        duLabel = new Label(); duLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); duLabel.setWrapText(true);
        erSieEsLabel = new Label(); erSieEsLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); erSieEsLabel.setWrapText(true);
        wirLabel = new Label(); wirLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); wirLabel.setWrapText(true);
        ihrLabel = new Label(); ihrLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); ihrLabel.setWrapText(true);
        sieSieLabel = new Label(); sieSieLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); sieSieLabel.setWrapText(true);
        perfectLabel = new Label(); perfectLabel.setStyle("-fx-font-size: 14px; -fx-alignment: center-left;"); perfectLabel.setWrapText(true);

        Button nextButton = new Button("Next");
        nextButton.setOnAction(e -> {
            currentIndex = (currentIndex + 1) % flashcards.size();
            showingBase = true; showingIchPlural = true; showingDu = true; showingErSieEs = true;
            showingWir = true; showingIhr = true; showingSieSie = true; showingPerfect = true;
            updateCard();
        });

        Button flipBaseButton = new Button("Base");
        flipBaseButton.setOnAction(e -> {
            showingBase = !showingBase;
            updateCard();
        });

        flipIchPluralButton = new Button();
        flipIchPluralButton.setOnAction(e -> {
            showingIchPlural = !showingIchPlural;
            updateCard();
        });

        flipDuButton = new Button("du");
        flipDuButton.setOnAction(e -> {
            showingDu = !showingDu;
            updateCard();
        });

        flipErSieEsButton = new Button("er/sie/es");
        flipErSieEsButton.setOnAction(e -> {
            showingErSieEs = !showingErSieEs;
            updateCard();
        });

        flipWirButton = new Button("du");
        flipWirButton.setOnAction(e -> {
            showingWir = !showingWir;
            updateCard();
        });

        flipIhrButton = new Button("ihr");
        flipIhrButton.setOnAction(e -> {
            showingIhr = !showingIhr;
            updateCard();
        });

        flipSieSieButton = new Button("sie/Sie");
        flipSieSieButton.setOnAction(e -> {
            showingSieSie = !showingSieSie;
            updateCard();
        });

        flipPerfectButton = new Button("perfect");
        flipPerfectButton.setOnAction(e -> {
            showingPerfect = !showingPerfect;
            updateCard();
        });

        Button addButton = new Button("Add a Card");

        //Labels to the right side.


        HBox topBar = new HBox(addButton);
        topBar.setAlignment(Pos.TOP_LEFT);
        topBar.setPadding(new Insets(10));

        VBox topLabel = new VBox(20, topBar, flashcardLabel);
        topLabel.setStyle("-fx-alignment: center;");
        VBox bottomButton = new VBox(20, nextButton);
        bottomButton.setStyle("-fx-padding: 20px; -fx-alignment: bottom-center;");

        GridPane centerGrid = new GridPane();
        centerGrid.setHgap(20);
        centerGrid.setVgap(10);
        centerGrid.setPadding(new Insets(20));

        //Buttons.
        centerGrid.add(flipBaseButton, 0, 0);
        centerGrid.add(flipIchPluralButton, 0, 1);
        centerGrid.add(flipDuButton, 0, 2);
        centerGrid.add(flipErSieEsButton, 0, 3);
        centerGrid.add(flipWirButton, 0, 4);
        centerGrid.add(flipIhrButton, 0, 5);
        centerGrid.add(flipSieSieButton, 0, 6);
        centerGrid.add(flipPerfectButton, 0, 7);

        //Labels.
        centerGrid.add(baseLabel, 1, 0);
        centerGrid.add(ichPluralLabel, 1, 1);
        centerGrid.add(duLabel, 1, 2);
        centerGrid.add(erSieEsLabel, 1, 3);
        centerGrid.add(wirLabel, 1, 4);
        centerGrid.add(ihrLabel, 1, 5);
        centerGrid.add(sieSieLabel, 1, 6);
        centerGrid.add(perfectLabel, 1, 7);

        BorderPane root = new BorderPane();
        root.setTop(topLabel);
        root.setBottom(bottomButton);
        root.setCenter(centerGrid);

        addButton.setOnAction(e -> openAddCardForm());

        stage.setScene(new Scene(root, 300, 600));
        stage.setTitle("Flashcard App");
        stage.show();
        
        // Only call updateCard if we have flashcards
        if (!flashcards.isEmpty()) {
            updateCard();
        }
    }

    private void openAddCardForm() {
        Stage formStage = new Stage();
        formStage.setTitle("Add Flashcard");

        // Radio buttons
        ToggleGroup typeGroup = new ToggleGroup();
        RadioButton nounBtn = new RadioButton("Noun");
        RadioButton verbBtn = new RadioButton("Verb");
        nounBtn.setToggleGroup(typeGroup);
        verbBtn.setToggleGroup(typeGroup);
        nounBtn.setSelected(true);

        HBox typeBox = new HBox(10, nounBtn, verbBtn);

        // 9 labeled text fields
        String[] labels = {"Question", "Base", "ich/plural", "du", "er/sie/es", "wir", "ihr", "sie/Sie", "perfect"};
        List<TextField> textFields = new ArrayList<>();
        VBox fieldsBox = new VBox(5);
        for (String label : labels) {
            Label l = new Label(label);
            TextField tf = new TextField();
            textFields.add(tf);
            fieldsBox.getChildren().add(new VBox(2, l, tf));
        }

        // Submit button
        Button submit = new Button("Add");
        submit.setOnAction(e -> {
            String type = ((RadioButton) typeGroup.getSelectedToggle()).getText().toLowerCase();

            StringBuilder values = new StringBuilder();
            values.append(type);
            for(TextField tf : textFields) {
                values.append(",").append(tf.getText());
            }

            addCard(flashcardsFile, values.toString());

            formStage.close();
        });

        VBox layout = new VBox(15, typeBox, fieldsBox, submit);
        layout.setPadding(new Insets(15));
        layout.setAlignment(Pos.CENTER_LEFT);

        Scene scene = new Scene(layout, 350, 600);
        formStage.setScene(scene);
        formStage.initModality(Modality.APPLICATION_MODAL);
        formStage.showAndWait();
    }

    // Modify the updateCard method to handle empty flashcards list
    private void updateCard() {

        Flashcard card = flashcards.get(currentIndex);
        //Set buttons text.
        flashcardLabel.setText(card.getQuestion());
        if(card.getNounOrVerb().equals("noun")) {
            flipIchPluralButton.setText("plural");
            flipDuButton.setText("");
            flipErSieEsButton.setText("");
            flipWirButton.setText("");
            flipIhrButton.setText("");
            flipSieSieButton.setText("");
            flipPerfectButton.setText("");
        } else if(card.getNounOrVerb().equals("verb")) {
            flipIchPluralButton.setText("ich");
            flipDuButton.setText("du");
            flipErSieEsButton.setText("er/sie/es");
            flipWirButton.setText("wir");
            flipIhrButton.setText("ihr");
            flipSieSieButton.setText("sie/Sie");
            flipPerfectButton.setText("perfect");
        } else {
            flipIchPluralButton.setText("");
        }

        //Set labels text.
        baseLabel.setText(showingBase ? "" : card.getBase());
        ichPluralLabel.setText(showingIchPlural ? "" : card.getIchPlural());
        duLabel.setText(showingDu ? "" : card.getDu());
        erSieEsLabel.setText(showingErSieEs ? "" : card.getErSieEs());
        wirLabel.setText(showingWir ? "" : card.getWir());
        ihrLabel.setText(showingIhr ? "" : card.getIhr());
        sieSieLabel.setText(showingSieSie ? "" : card.getSieSie());
        perfectLabel.setText(showingPerfect ? "" : card.getPerfect());
    }

    public static void main(String[] args) {
        launch(args);
    }
}