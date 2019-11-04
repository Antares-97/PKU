package Classifier_text;
import java.io.File;

import sun.nio.cs.FastCharsetProvider;
import weka.classifiers.Classifier;
import weka.classifiers.Evaluation;
import weka.classifiers.bayes.BayesNet;
import weka.classifiers.bayes.NaiveBayes;
import weka.classifiers.evaluation.NominalPrediction;
import weka.classifiers.functions.MultilayerPerceptron;
import weka.classifiers.functions.SimpleLogistic;
import weka.classifiers.lazy.IBk;
import weka.classifiers.lazy.KStar;
import weka.classifiers.pmml.consumer.SupportVectorMachineModel;
import weka.classifiers.rules.DecisionTable;
import weka.classifiers.rules.PART;
import weka.classifiers.trees.DecisionStump;
import weka.classifiers.trees.J48;
import weka.classifiers.trees.RandomForest;
import weka.core.FastVector;
import weka.core.Attribute;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.converters.ArffLoader;
import weka.filters.Filter;
import weka.filters.unsupervised.attribute.Remove;
import java.io.BufferedWriter;
import java.io.FileWriter;

import java.util.*;
import java.io.*;

import com.hankcs.hanlp.HanLP;
import com.hankcs.hanlp.seg.common.Term;
import com.hankcs.hanlp.seg.*;
import com.hankcs.hanlp.seg.NShort.*;


public class HW3_1500011370
{
    public static void maketraindata(String TrainingSetDir)
    {
        String[] wordlist = new String[]{
                "之","其","或","亦","方","于","即","皆","因","仍","故","尚","呢","了",
                "的","着","一","不","乃","呀","吗","咧","啊","把","让","向","往","是",
                "在","越","再","更","比","很","偏","别","好","可","便","就","但","尔",
                "又","也","都","要","这","那","你","我","他","来","去","道","说","吾",
        };

        try
        {
            File file = new File("TrainingSetData.txt");
            if(!file.exists())
            {
                file.createNewFile();
            }
            FileWriter fw = new FileWriter(file,false); //清空文件重写，要想追加写入，则将FileWriter构造函数中第二个参数变为true
            BufferedWriter bw = new BufferedWriter(fw);

            bw.write("@relation trainset_label\n\n");
            bw.write("@attribute zhi real\n");
            bw.write("@attribute qi real\n");
            bw.write("@attribute huo real\n");
            bw.write("@attribute yi1 real\n");
            bw.write("@attribute fang real\n");
            bw.write("@attribute yu real\n");
            bw.write("@attribute ji real\n");
            bw.write("@attribute jie real\n");
            bw.write("@attribute yin real\n");
            bw.write("@attribute reng real\n");
            bw.write("@attribute gu real\n");
            bw.write("@attribute shang real\n");
            bw.write("@attribute ne real\n");
            bw.write("@attribute liao real\n");
            bw.write("@attribute de real\n");
            bw.write("@attribute zhe1 real\n");
            bw.write("@attribute yi2 real\n");
            bw.write("@attribute bu real\n");
            bw.write("@attribute nai real\n");
            bw.write("@attribute ya real\n");
            bw.write("@attribute ma real\n");
            bw.write("@attribute lie real\n");
            bw.write("@attribute a real\n");
            bw.write("@attribute ba real\n");
            bw.write("@attribute rang real\n");
            bw.write("@attribute xiang real\n");
            bw.write("@attribute wang real\n");
            bw.write("@attribute shi real\n");
            bw.write("@attribute zai1 real\n");
            bw.write("@attribute yue real\n");
            bw.write("@attribute zai2 real\n");
            bw.write("@attribute geng real\n");
            bw.write("@attribute bi real\n");
            bw.write("@attribute hen real\n");
            bw.write("@attribute pian real\n");
            bw.write("@attribute bie real\n");
            bw.write("@attribute hao real\n");
            bw.write("@attribute ke real\n");
            bw.write("@attribute bian real\n");
            bw.write("@attribute jiu real\n");
            bw.write("@attribute dan real\n");
            bw.write("@attribute er real\n");
            bw.write("@attribute you real\n");
            bw.write("@attribute ye real\n");
            bw.write("@attribute dou real\n");
            bw.write("@attribute yao real\n");
            bw.write("@attribute zhe2 real\n");
            bw.write("@attribute na real\n");
            bw.write("@attribute ni real\n");
            bw.write("@attribute wo real\n");
            bw.write("@attribute ta real\n");
            bw.write("@attribute lai real\n");
            bw.write("@attribute qu real\n");
            bw.write("@attribute dao real\n");
            bw.write("@attribute shuo real\n");
            bw.write("@attribute wu real\n");
            bw.write("@attribute label {0,1,2}\n\n");
            bw.write("@data\n\n");

            BufferedReader trainingfile = null;
            try
            {
                trainingfile = new BufferedReader(new FileReader(TrainingSetDir));
            }
            catch (FileNotFoundException ex)
            {
                System.err.println("File not found:" + TrainingSetDir);
            }
            Map<String, Character> TrainingSet = new HashMap<String,Character>();
            try
            {
                String str = trainingfile.readLine();      //每次读一行
                while (str != null)
                {
                    Character label = str.charAt(0);
                    String datadir = str.substring(2);
                    TrainingSet.put(datadir,label);
                    //System.out.println(datadir+" "+label);
                    str = trainingfile.readLine();
                }
            }
            catch (Exception e) { }

            Set<Map.Entry<String, Character>> set =TrainingSet.entrySet();
            Iterator<Map.Entry<String, Character>> itera_Entry = set.iterator();

            while(itera_Entry.hasNext())
            {
                Map.Entry<String, Character> mapEntry =itera_Entry.next();
                String filename = mapEntry.getKey();
                //System.out.println(filename);
                Character label = mapEntry.getValue();
                double[] frequency =new double[wordlist.length];
                for (int l=0; l<frequency.length;l++) { frequency[l]=0; }
                double totalword = 0;
                BufferedReader datafile = null;
                try
                {
                    datafile = new BufferedReader(new FileReader(filename));
                }
                catch (FileNotFoundException ex)
                {
                    System.err.println("File not found:" + filename);
                }

                Segment nShortSegment = new NShortSegment().enableCustomDictionary(true).enablePlaceRecognize(true).enableOrganizationRecognize(true).enablePartOfSpeechTagging(false);
                try
                {
                    List<Term> termList = null;
                    String str = datafile.readLine();      //每次读一行
                    while(str!=null)
                    {
                        termList = nShortSegment.seg(str);    //对该行进行分词
                        //System.out.println(termList);
                        for (int j=0 ; j < termList.size() ; j++)
                        {
                            for(int k=0; k < wordlist.length; k++)
                            {
                                //System.out.println(wordlist[k]);
                                if (termList.get(j).word.equals(wordlist[k]))
                                {
                                    frequency[k]++;
                                    totalword++;
                                    //System.out.println(termList.get(j).word);
                                }
                            }
                            // System.out.print("\n");
                        }
                        str = datafile.readLine();
                    }
                }
                catch (Exception e) { }
                //System.out.print(filename+",");
                for (int l=0 ; l<frequency.length ; l++)
                {
                    frequency[l]=10000*frequency[l]/totalword;
                    bw.write(String.format("%.1f,", frequency[l]));
                    //System.out.print(String.format("%.1f,", frequency[l]));
                }
                bw.write(String.format("%c\n", label));
                //System.out.print(String.format("%c\n", label));
            }

            bw.close();
            fw.close();
        } catch (Exception e) { }

    }

    public static void maketestdata(String TestingSetDir)
    {
        String[] wordlist = new String[]{
                "之","其","或","亦","方","于","即","皆","因","仍","故","尚","呢","了",
                "的","着","一","不","乃","呀","吗","咧","啊","把","让","向","往","是",
                "在","越","再","更","比","很","偏","别","好","可","便","就","但","尔",
                "又","也","都","要","这","那","你","我","他","来","去","道","说","吾",
        };

        try
        {
            File file = new File("TestingSetData.txt");
            if(!file.exists())
            {
                file.createNewFile();
            }
            FileWriter fw = new FileWriter(file,false); //清空文件重写，要想追加写入，则将FileWriter构造函数中第二个参数变为true
            BufferedWriter bw = new BufferedWriter(fw);

            bw.write("@relation testset_label\n\n");
            bw.write("@attribute zhi real\n");
            bw.write("@attribute qi real\n");
            bw.write("@attribute huo real\n");
            bw.write("@attribute yi1 real\n");
            bw.write("@attribute fang real\n");
            bw.write("@attribute yu real\n");
            bw.write("@attribute ji real\n");
            bw.write("@attribute jie real\n");
            bw.write("@attribute yin real\n");
            bw.write("@attribute reng real\n");
            bw.write("@attribute gu real\n");
            bw.write("@attribute shang real\n");
            bw.write("@attribute ne real\n");
            bw.write("@attribute liao real\n");
            bw.write("@attribute de real\n");
            bw.write("@attribute zhe1 real\n");
            bw.write("@attribute yi2 real\n");
            bw.write("@attribute bu real\n");
            bw.write("@attribute nai real\n");
            bw.write("@attribute ya real\n");
            bw.write("@attribute ma real\n");
            bw.write("@attribute lie real\n");
            bw.write("@attribute a real\n");
            bw.write("@attribute ba real\n");
            bw.write("@attribute rang real\n");
            bw.write("@attribute xiang real\n");
            bw.write("@attribute wang real\n");
            bw.write("@attribute shi real\n");
            bw.write("@attribute zai1 real\n");
            bw.write("@attribute yue real\n");
            bw.write("@attribute zai2 real\n");
            bw.write("@attribute geng real\n");
            bw.write("@attribute bi real\n");
            bw.write("@attribute hen real\n");
            bw.write("@attribute pian real\n");
            bw.write("@attribute bie real\n");
            bw.write("@attribute hao real\n");
            bw.write("@attribute ke real\n");
            bw.write("@attribute bian real\n");
            bw.write("@attribute jiu real\n");
            bw.write("@attribute dan real\n");
            bw.write("@attribute er real\n");
            bw.write("@attribute you real\n");
            bw.write("@attribute ye real\n");
            bw.write("@attribute dou real\n");
            bw.write("@attribute yao real\n");
            bw.write("@attribute zhe2 real\n");
            bw.write("@attribute na real\n");
            bw.write("@attribute ni real\n");
            bw.write("@attribute wo real\n");
            bw.write("@attribute ta real\n");
            bw.write("@attribute lai real\n");
            bw.write("@attribute qu real\n");
            bw.write("@attribute dao real\n");
            bw.write("@attribute shuo real\n");
            bw.write("@attribute wu real\n");
            bw.write("@attribute label {0,1,2}\n\n");
            bw.write("@data\n\n");

            BufferedReader testingfile = null;
            try
            {
                testingfile = new BufferedReader(new FileReader(TestingSetDir));
            }
            catch (FileNotFoundException ex)
            {
                System.err.println("File not found:" + TestingSetDir);
            }

            Vector<String> TestingSet = new Vector<String>();

            try
            {
                String datadir = testingfile.readLine();      //每次读一行
                while (datadir != null)
                {
                    //Character label = str.charAt(0);
                    //String datadir = str.substring(2);
                    TestingSet.add(datadir);
                    //System.out.println(datadir+" "+label);
                    datadir = testingfile.readLine();
                }
            }
            catch (Exception e) { }

            for(int i=0; i<TestingSet.size(); i++)
            {
                String filename = TestingSet.get(i);
                double[] frequency =new double[wordlist.length];
                for (int l=0; l<frequency.length;l++) { frequency[l]=0; }
                double totalword = 0;
                BufferedReader datafile = null;
                try
                {
                    datafile = new BufferedReader(new FileReader(filename));
                }
                catch (FileNotFoundException ex)
                {
                    System.err.println("File not found:" + filename);
                }

                Segment nShortSegment = new NShortSegment().enableCustomDictionary(true).enablePlaceRecognize(true).enableOrganizationRecognize(true).enablePartOfSpeechTagging(false);
                try
                {
                    List<Term> termList = null;
                    String str = datafile.readLine();      //每次读一行
                    while(str!=null)
                    {
                        termList = nShortSegment.seg(str);    //对该行进行分词
                        //System.out.println(termList);
                        for (int j=0 ; j < termList.size() ; j++)
                        {
                            for(int k=0; k < wordlist.length; k++)
                            {
                                //System.out.println(wordlist[k]);
                                if (termList.get(j).word.equals(wordlist[k]))
                                {
                                    frequency[k]++;
                                    totalword++;
                                    //System.out.println(termList.get(j).word);
                                }
                            }
                            // System.out.print("\n");
                        }
                        str = datafile.readLine();
                    }
                }
                catch (Exception e) { }
                //System.out.print(filename+",");
                //bw.write(String.format("%.1f", frequency[0]));
                //System.out.print(String.format("%.1f", frequency[0]));
                for (int l=0 ; l<frequency.length ; l++)
                {
                    frequency[l]=10000*frequency[l]/totalword;
                    bw.write(String.format("%.1f,", frequency[l]));
                    //System.out.print(String.format("%.1f,", frequency[l]));
                }
                //bw.write(String.format("%c\n", label));
                //System.out.print(String.format("%c\n", label));
                bw.write("?\n");
                //System.out.print("?\n");
            }

            bw.close();
            fw.close();
        } catch (Exception e) { }

    }

    public static Evaluation classify(Classifier model, Instances trainingSet, Instances testingSet) throws Exception
    {
        Evaluation evaluation = new Evaluation(trainingSet);
        model.buildClassifier(trainingSet);
        try
        {
            File file = new File("HW3_1500011370.txt");
            if(!file.exists())
            {
                file.createNewFile();
            }
            FileWriter fw = new FileWriter(file,false); //清空文件重写，要想追加写入，则将FileWriter构造函数中第二个参数变为true
            BufferedWriter bw = new BufferedWriter(fw);
            for (int i=0; i<testingSet.size(); i++)
            {
                double predicted=model.classifyInstance(testingSet.get(i));
                //System.out.println("predicted:"+predicted);
                //System.out.println("Message classified as : " + testingSet.classAttribute().value((int)predicted));
                bw.write(testingSet.classAttribute().value((int)predicted)+"\n");       //输出测试文件
            }
            bw.close();
            fw.close();         //加了这俩终于写出来了
        }
        catch (Exception e) { }
        return evaluation;
    }

    public static double calculateAccuracy(FastVector predications)
    {
        double correct = 0;
        try
        {
            File file = new File("HW3_1500011370.txt");
            if(!file.exists())
            {
                file.createNewFile();
            }
            FileWriter fw = new FileWriter(file,false); //清空文件重写，要想追加写入，则将FileWriter构造函数中第二个参数变为true
            BufferedWriter bw = new BufferedWriter(fw);
            for (int i = 0; i < predications.size(); i++)
            {
                NominalPrediction np = (NominalPrediction) predications.elementAt(i);
                bw.write(String.format("%s\n", np.predicted()));
                //System.out.print(String.format("Predicted: %s, Actual: %s\n", np.predicted(), np.actual()));
                if (np.predicted() == np.actual())
                {
                    correct++;
                }
            }
        }
        catch (Exception e) { }
        return 100 * correct / predications.size();
    }

    public static Instances[][] crossValidationSplit(Instances data, int numberofFolds)
    {
        Instances[][] split = new Instances[2][numberofFolds];
        for (int i = 0; i < numberofFolds; i++)
        {
            split[0][i] = data.trainCV(numberofFolds, i);
            split[1][i] = data.testCV(numberofFolds, i);
        }
        return split;
    }

    public static void main(String[] args) throws Exception
    {
        //请修改这两个文件路径

        String TrainingSetDir = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Java/hw3/HW3_1500011370/TrainingSetDir.txt";
        String TestingSetDir = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Java/hw3/HW3_1500011370/TestingSetDir.txt";
        maketraindata(TrainingSetDir);
        maketestdata(TestingSetDir);

        BufferedReader datafile = null;
        String trainingfilename = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Java/hw3/HW3_1500011370/TrainingSetData.txt";
        try
        {
            datafile = new BufferedReader(new FileReader(trainingfilename));
        } catch (FileNotFoundException ex)
        {
            System.err.println("File not Found: " + trainingfilename);
        }

        Instances trainingdata = new Instances(datafile);
        trainingdata.setClassIndex(trainingdata.numAttributes()-1);

        String testingfilename = "/Users/macbookair/iCloud/Desktop/Daily/Second Major/CS/Java/hw3/HW3_1500011370/TrainingSetData.txt";
        try
        {
            datafile = new BufferedReader(new FileReader(testingfilename));
        } catch (FileNotFoundException ex)
        {
            System.err.println("File not Found: " + testingfilename);
        }
        Instances testingdata = new Instances(datafile);
        testingdata.setClassIndex(testingdata.numAttributes()-1);

        //Instances[][] split = crossValidationSplit(data, 10);
        //Instances[] trainingSplits = split[0];
        //Instances[] testingSplits = split[1];

        Classifier[] models =
                {
                        //new J48(),
                        //new DecisionTable(),
                        //new BayesNet(),
                        //new KStar(),
                        //new NaiveBayes(),
                        new SimpleLogistic(),
                        //new RandomForest(),
                        //new MultilayerPerceptron()
                };

        for (int i = 0; i < models.length; i++)
        {
            FastVector predictions = new FastVector();
            Evaluation validation = classify(models[i], trainingdata, testingdata);     //输出测试文件
            //predictions.appendElements(validation.predictions());

            //double accuracy = calculateAccuracy(predictions);
            //System.out.println("Accuracy of " + models[i].getClass().getSimpleName() + ": "
                    //+ String.format("%.3f%%", accuracy)
                   // + "\n----------------------------");
        }
    }
}
