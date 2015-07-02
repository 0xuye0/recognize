#include <cstdlib>
#include "SmsCal.h"
#include <sstream>
#include <cmath>


//���캯��
SmsClassifyCalculate::SmsClassifyCalculate()
{

}

//��������
SmsClassifyCalculate::~SmsClassifyCalculate()
{

}

//ԭ�ַ�������ַ����ж�
int SmsClassifyCalculate::KeyOriginalJudge(const vector<int>& Sequence)
{
	int count=0;
	for(int i=0;i<Sequence.size();i++)
	{
		if(Sequence[i]>0)
		{
			count =1;

			break;
		}
}
	return count;
}
//˫�ַ��ؼ����ж�
int SmsClassifyCalculate::KeyDoubleJudge(const vector<int>& Sequence)
{
	if(Sequence.size()==2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//˫�ַ��ؼ��ʼ���
float SmsClassifyCalculate::KeyDoubleCal(const vector<int>& Sequence)
{
	int temp;
	temp=abs(Sequence[1])-abs(Sequence[0]);
	//if(abs(temp)>6)
	//{
	//	return 0;
	//}
	//else
	{
		float ScoreDoubleTemp= ((abs(temp)+1.4)*exp(-0.42*(abs(temp)+1.4) + 1) ) / 2.3809;
//		float ScoreDoubleTemp= ((abs(temp))*exp(-1*(abs(temp)) + 1) );
		return ScoreDoubleTemp;
	}
}
//��������Ƿ�Ȳ�
//���룺����vector<int>
//�������Ϊ�Ȳ����з���1�����򷵻�0��
int SmsClassifyCalculate::KeyArithmetic(const vector<int>& Sequence)
{
	int temp;
	temp=abs(Sequence[1])-abs(Sequence[0]);
	int i;
	for(i=1;i < Sequence.size();i++)
	{
		if(abs(Sequence[i])-abs(Sequence[i-1])!=temp)
		{return 0;}
	}
	return temp;
}

//�ȼ��Ȩֵ����
float SmsClassifyCalculate::KeySpaceCal(const vector<int>& Sequence)
{
	float temp;
	float temp2=1;
	temp=abs(abs(Sequence[1])-abs(Sequence[0]));
	if(temp>4&&temp<10)//�������
	{
		float ScoreSpaced1=1.5;
		return ScoreSpaced1;
	}
	else
	{
    int KeyCharsize=Sequence.size()-1;
	float ScoreSpaced2=exp(-(temp-1))*(KeyCharsize+1)/KeyCharsize;//ȡ�ȼ���м���ľ���ֵ��ֵΪ�ݴ�
	return ScoreSpaced2;
	}
}

//���������ж�
int SmsClassifyCalculate::KeyVerticalJudge(const vector<int> &Sequence)
{
	float temp;
	temp=abs(abs(Sequence[1])-abs(Sequence[0]));
	if(temp>4&&temp<10)//�����ж�
	{
		return 1;
	}
	else
	{
		return 0;
	}

}
//���Ƶȼ��Ȩֵ����
float SmsClassifyCalculate::KeyApproxCal(const vector<int>& Sequence)
{
	float ScoreApprox;
	int max;
	int min;
	int u;
	std::vector<int> score;
	for(u=0;u<Sequence.size()-1;u++)
	{
		int num=0;
		num=abs(Sequence[u+1])-abs(Sequence[u]);
		score.push_back(num);
	}
	max=score[0];
	min=score[0];
	int w;
	for(w=0;w<score.size();w++)
	{
		if(score[w]>=max)
		{
			max=score[w];
		}
		else
		{
			min=score[w];
		}
	}
	if(max-min>2)
	{return ScoreApprox=0;}
	else
	{
		int temp=0;
		int i;
		for(i=1;i<Sequence.size();i++)
		{
			temp=temp+abs(abs(Sequence[i])-abs(Sequence[i-1]));
		}
		float s= temp/(Sequence.size()-1);
		ScoreApprox= exp(-(fabs(s)-1));   //ȡ���ƽ��ֵ�ľ���ֵ��ֵΪ�ݴ�
		return ScoreApprox;
	}
}

//��Ⲣ�����Ƿ�Ϊ����
//���룺����vector<int>
//�������Ϊ�������з���1�����򷵻�0.6��
float SmsClassifyCalculate::ReverseJudgeAndCal(const vector<int> &Sequence)
{
	int i=1;
	int temp;
	temp=abs(Sequence[i])-abs(Sequence[i-1]);
	float ScoreReverse=0.6;//�˴�����Ȩֵδ��
	if(temp<0)
	{return ScoreReverse;}
	else
	{
		return 1;
	}
}

//���ڳ��ֹؼ��ʿ��йؼ��ʵĶ��Ÿ���һ����Ȩֵ
float SmsClassifyCalculate::KeyAppearance(const std::vector<int> &Sequence)
{
	float ScoreAppearance=0;
	for(int i=0;i<Sequence.size();i++)
	{
		if(Sequence[i]>0)
		{
            ScoreAppearance=0.05;
			break;
		}
	}
	return ScoreAppearance;
}

//��Ȩֵ����
float SmsClassifyCalculate::KeyScoreCal()
{	
    float VWeight;
    string tempweight = origin_position + "VariantWeight.txt";
    ifstream txy(tempweight.c_str());
    if (!txy) {
        throw 5;
    } else {
      string line = "";
      getline(txy, line);
      stringstream temp;
      temp << line;
      temp >> VWeight;
    }
	//4������Ȩֵ
	float ScoreDouble(0);
	float ScoreApprox(0);
	float ScoreSpaced(0);
	float ScoreReverse(0);
	float clsScoreTemp=0;

	std::vector<double> FeatureTagging(7,0);
	
	//7��������ǩ�ж�ֵ
	int FeatureOriginal(0);
	int FeatureChange(0);
	int FeatureDouble(0);
	int FeatureApprox(0);
	int FeatureSpaced(0);
	int FeatureReverse(0);
	int FeatureVertical(0);


	float addTemp;
	multimap<int,KeySearch>::iterator it;
	for(it=ResultKeyWord.begin();it!=ResultKeyWord.end();it++)
	{
		addTemp=0;
		if(KeyDoubleJudge((*it).second.position))
		{
			ScoreDouble=KeyDoubleCal((*it).second.position);
			if(ScoreDouble!=0)//?
			{
			FeatureDouble=FeatureDouble+1;
			}
			addTemp=addTemp+ScoreDouble;
		}
		else
		{
		if(!KeyArithmetic((*it).second.position))
		{
			ScoreApprox=KeyApproxCal((*it).second.position);
			if(ScoreApprox!=0)
			{
				FeatureApprox=FeatureApprox+1;
			}
			addTemp=addTemp+ScoreApprox;
		}
		else 
		{
			ScoreSpaced=KeySpaceCal((*it).second.position);
			if(ScoreSpaced!=0)
			{
				FeatureSpaced=FeatureSpaced+1;
			}
			addTemp=addTemp+ScoreSpaced;
		}
		}
		ScoreReverse= ReverseJudgeAndCal((*it).second.position);
        float tempweight = 1;
        for (int i = 0; i < (*it).second.position.size() - KeyOriginalJudge((*it).second.position); ++i) {
            tempweight *= VWeight;
        }
		clsScoreTemp=clsScoreTemp+tempweight*((*it).second.keywordvalue*ScoreReverse*addTemp+KeyAppearance((*it).second.position));
		
		
		if (ScoreReverse!=1&&addTemp!=0)
		{FeatureReverse=FeatureReverse+1;}

		if((!KeyOriginalJudge((*it).second.position))&&(addTemp!=0))
		{
			FeatureOriginal=FeatureOriginal+1;
		}
		else if((KeyOriginalJudge((*it).second.position))&&(addTemp!=0))
		{
			FeatureChange=FeatureChange+1;
		}
		if((KeyVerticalJudge((*it).second.position))&&(addTemp!=0))
		{
			FeatureVertical=FeatureVertical+1;
		}
	}
	FeatureTagging[0]=FeatureOriginal;
	FeatureTagging[1]=FeatureChange;
	FeatureTagging[2]=FeatureDouble;
	FeatureTagging[3]=FeatureSpaced;
	FeatureTagging[4]=FeatureApprox;
	FeatureTagging[5]=FeatureReverse;
	FeatureTagging[6]=FeatureVertical;
	ResultType.FeatureType = FeatureTagging;

	return clsScoreTemp;
}

//�о� ���洢�о���� ��Ϊ3�� �к� ���� �޺�
//���õ������ǩ���
int SmsClassifyCalculate::TypeJudge()
{
	float clsScoreTemp=KeyScoreCal();
	float clsScoreMax = 0.66;
	if(clsScoreTemp<clsScoreMax)
	{
		ResultType.ScoreFinal=clsScoreTemp;
		ResultType.type=0;
	}
	else
	{
		ResultType.ScoreFinal=clsScoreTemp;
        ResultType.type=2;
	}
	return ResultType.type;
}

//�������
int SmsClassifyCalculate::PrintResult()
{
	std::cout<<"���յ÷�:"<<ResultType.ScoreFinal<<std::endl;
	std::cout<<"������:"<<ResultType.type<<std::endl;
	//std::cout<<"�����ǩ:"<<"˫�ַ�:"<<ResultType.FeatureType[0]<<"�ȼ��:"<<ResultType.FeatureType[1]<<"���ƵȾ�:"<<ResultType.FeatureType[2]<<"����:"<<ResultType.FeatureType[3]<<std::endl;
	return 0;
}

//process
//���Ŷȷ���ģ����⴦��ӿں���
int SmsClassifyCalculate::process(MessageStructure & sms)
{
	ResultKeyWord=sms.ResultKeyWord;//��MessageStructure sms���Žṹ���л�ȡ������ȡģ��Ĵ�����
	//TypeJudge();//���Ŷȼ��㡢�жϴ���
	if(TypeJudge()==5)
	{
		return 5;
	}
	else{
	sms.value=ResultType.ScoreFinal;//���Ŷȼ����������Ϣ�ṹ��
	sms.type=ResultType.type;//���Ŷȼ����������Ϣ�ṹ��
	sms.FeatureTag=ResultType.FeatureType;
	return 1;
	}
}
