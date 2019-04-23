package main;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import exceptions.ParserException;
import model.Table;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.misc.ParseCancellationException;
import org.antlr.v4.runtime.tree.ParseTreeWalker;
import parser.*;
import parser.XtraCompiler;
import serializers.TableSerializer;

import org.apache.commons.cli.*;


import java.io.*;

public class MainClass {
    public static void main(final String[] args) {

        Options options = new Options();
        Option option = Option.builder("i")
                .argName("file")
                .hasArg()
                .required()
                .longOpt("input")
                .desc("input file")
                .build();

        options.addOption(option);

        option = Option.builder("o")
                .argName("file")
                .hasArg()
                .required()
                .longOpt("output")
                .desc("output json file")
                .build();

        options.addOption(option);


        CommandLineParser parserCmd = new DefaultParser();
        CommandLine cmd = null;

        try {
            cmd = parserCmd.parse( options, args);
        } catch (Exception e){
            help(options);

        }

        String inputFile = cmd.getOptionValue("i");
        String outputFile = cmd.getOptionValue("o");

        xtraLexer lexer = null;
        try {
            lexer = new xtraLexer(CharStreams.fromFileName(inputFile));
        } catch (IOException e) {
            System.err.println("[XTRAC] Unable to find file "+ inputFile);
            System.exit(1);
        }
        lexer.removeErrorListeners();
        lexer.addErrorListener(new XtraErrorListener());
        xtraParser parser = new xtraParser(new CommonTokenStream(lexer));
        parser.removeErrorListeners();
        parser.addErrorListener(new XtraErrorListener());
        ParseTreeWalker walker = new ParseTreeWalker();
        XtraCompiler listener = new XtraCompiler();
        try
        {
            walker.walk(listener, parser.program());
        }
        catch (ParseCancellationException e)
        {
            System.err.print("[XTRAC] Error while compiling: ");
            e.printStackTrace();
            System.out.println(e.getCause());
            System.exit(1);
        }

        final GsonBuilder gsonBuilder = new GsonBuilder();
        gsonBuilder.registerTypeAdapter(Table.class, new TableSerializer());
        gsonBuilder.setPrettyPrinting();
        final Gson gson = gsonBuilder.create();

        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter(outputFile));
            writer.write(gson.toJson(listener.getTable()).toString());
            writer.close();
        } catch (IOException e) {
            System.err.println("[XTRAC] Unable to create file "+outputFile);
            System.exit(1);
        } catch (ParserException e) {
            System.err.print("[XTRAC] Error while compiling: ");
            e.printStackTrace();
            System.out.println(e.getCause());
            System.exit(1);
        }

    }

    /**
     * Stampa le opzioni con cui è possibile avviare la simulazione
     * @param options Variabile con le opzioni utilizzabili
     */
    private static void help(Options options) {
        HelpFormatter formater = new HelpFormatter();
        formater.printHelp("xtra", options);
        System.exit(-1);

    }


}
