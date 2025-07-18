package com.dlg.FlashCard;

public class Flashcard {
    private final String nounOrVerb, question, base, ichPlural, du, erSieEs, wir, ihr, sieSie, perfect;

    public Flashcard(String nounOrVerb, String question, String base, String ichPlural) {
       this(nounOrVerb, question, base, ichPlural, "", "", "", "", "", "");
    }

    public Flashcard(String nounOrVerb, String question, String base, String ichPlural,
                     String du, String erSieEs, String wir, String ihr, String sieSie,  String perfect) {
        this.nounOrVerb = nounOrVerb;
        this.question = question;
        this.base = base;
        this.ichPlural = ichPlural;
        this.du = du;
        this.erSieEs = erSieEs;
        this.wir = wir;
        this.ihr = ihr;
        this.sieSie = sieSie;
        this.perfect = perfect;
    }

    public String getNounOrVerb() {
        return nounOrVerb;
    }

    public String getQuestion() {
        return question;
    }

    public String getBase() {
        return base;
    }

    public String getIchPlural() {
        return ichPlural;
    }

    public String getDu() {
        return du;
    }

    public String getErSieEs() {
        return erSieEs;
    }

    public String getWir() {
        return wir;
    }

    public String getIhr() {
        return ihr;
    }

    public String getSieSie() {
        return sieSie;
    }

    public String getPerfect() {
        return perfect;
    }
}

