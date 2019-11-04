package hw1;

public class HW1_1500011370 
{
    public static int Answer(int i,int j)
    {
        int ans=(i>>(j<<1))&3;
        return ans;
    }
    public static void main(String[]args)
    {
        for(int i=0;i<1<<20;i++)
        {
            int[] Answers=new int [10];
            for(int j=0;j<10;j++) Answers[j]=HW1_1500011370.Answer(i,j);
            int[] ABCD=new int [4];
            int max=0;
            int min=10;
            int min_index=0;
            for(int j=0;j<10;j++) ABCD[HW1_1500011370.Answer(i,j)]++;
            for(int j=0;j<4;j++) 
            {
                if(ABCD[j]>max) 
                {
                    max=ABCD[j];
                }
            }
            for(int j=0;j<4;j++) 
            {
                if(ABCD[j]<min) 
                {
                    min=ABCD[j];
                    min_index=j;
                }
            }
            if((Answers[1]==0&&Answers[4]==2)||(Answers[1]==1&&Answers[4]==3)||(Answers[1]==2&&Answers[4]==0)||(Answers[1]==3&&Answers[4]==1))
                if((Answers[2]==0&&Answers[2]!=Answers[5]&&Answers[2]!=Answers[1]&&Answers[2]!=Answers[3])||(Answers[2]==1&&Answers[5]!=Answers[2]&&Answers[5]!=Answers[1]&&Answers[5]!=Answers[3])||(Answers[2]==2&&Answers[1]!=Answers[2]&&Answers[1]!=Answers[5]&&Answers[1]!=Answers[3])||(Answers[2]==3&&Answers[3]!=Answers[2]&&Answers[3]!=Answers[5]&&Answers[3]!=Answers[2]))
                    if(Answers[3]==0&&Answers[0]==Answers[5]||Answers[3]==1&&Answers[1]==Answers[6]||Answers[3]==2&&Answers[0]==Answers[8]||Answers[3]==3&&Answers[5]==Answers[9])
                        if(Answers[4]==0&&Answers[7]==0||Answers[4]==1&&Answers[3]==1||Answers[4]==2&&Answers[8]==2||Answers[4]==3&&Answers[6]==3)
                            if(Answers[5]==0&&Answers[7]==Answers[1]&&Answers[7]==Answers[3]||Answers[5]==1&&Answers[7]==Answers[0]&&Answers[7]==Answers[5]||Answers[5]==2&&Answers[7]==Answers[2]&&Answers[7]==Answers[9]||Answers[5]==3&&Answers[7]==Answers[4]&&Answers[7]==Answers[8])
                                if(Answers[6]==0&&min_index==2||Answers[6]==1&&min_index==1||Answers[6]==2&&min_index==0||Answers[6]==3&&min_index==3)
                                    if(Answers[7]==0&&Math.abs(Answers[6]-Answers[0])!=1||Answers[7]==1&&Math.abs(Answers[4]-Answers[0])!=1||Answers[7]==2&&Math.abs(Answers[1]-Answers[0])!=1||Answers[7]==3&&Math.abs(Answers[9]-Answers[0])!=1)
                                        if(Answers[8]==0&&(Answers[0]==Answers[5])!=(Answers[5]==Answers[4])||Answers[8]==1&&(Answers[0]==Answers[5])!=(Answers[9]==Answers[4])||Answers[8]==2&&(Answers[0]==Answers[5])!=(Answers[1]==Answers[4])||Answers[8]==3&&(Answers[0]==Answers[5])!=(Answers[8]==Answers[4]))
                                            if(Answers[9]==0&&(max-min==3)||Answers[9]==1&&(max-min==2)||Answers[9]==2&&(max-min==4)||Answers[9]==3&&(max-min==1))
                                            {
                                                System.out.print("1500011370");
                                                for(int j=0;j<10;j++) 
                                                {
                                                	 	if(Answers[j]==0) System.out.print('A');
                                                	 	if(Answers[j]==1) System.out.print('B');
                                                	 	if(Answers[j]==2) System.out.print('C');
                                                	 	if(Answers[j]==3) System.out.print('D');
                                                }
                                                System.out.print('\n');
                                                continue;
                                            }
                                        continue;
        }
    }
}
