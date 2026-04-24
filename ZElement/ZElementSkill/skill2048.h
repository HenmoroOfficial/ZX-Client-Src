#ifndef __CPPGEN_GNET_SKILL2048
#define __CPPGEN_GNET_SKILL2048
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2048:public Skill
    {
      public:
        enum
        { SKILL_ID = 2048 };
          Skill2048 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2048Stub:public SkillStub
    {
      public:
#ifdef _SKILL_SERVER
        class State1:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 800;
            }
            void Calculate (Skill * skill) const
            {
                skill->GetPlayer ()->SetPray (1);
            }
            bool Cancel (Skill * skill) const
            {
                return 1;
            }
        };
#endif
#ifdef _SKILL_SERVER
        class State2:public SkillStub::State
        {
          public:
            int GetTime (Skill * skill) const
            {
                return 200;
            }
            void Calculate (Skill * skill) const
            {
                skill->SetRatio (0.5 + 0.05 * skill->GetLevel ());
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2048Stub ():SkillStub (2048)
        {
            occupation = 148;
            name = L"º®±ùÈÐII";
            nativename = "º®±ùÈÐII";
            icon = "º®±ùÈÐII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 1;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 9;
            clearmask = 129;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 1;
            succeedskillcolornum = 1;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 0;
            guide = false;
            summon_id = 0;
            trigger = false;
            castInPreSkill = false;
            preCastSkillId = 0;
            darkLightType = 0;
            followskill = 0;
            targetcnt = 0;
            excludemonstercnt = 0;
            skillelemsnum = 0;
            skillelemsfactor = 0;
            preskillnum = 1;
            talent[0] = 252;
            talent_size = 1;
            action[0] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[1] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[2] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[3] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[4] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[5] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[6] = "¹íÍõ×Ú_º®±ùÈÐ";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[16] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[17] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[18] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[19] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[20] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[21] = "¹íÍõ×Ú_Æï³Ë_º®±ùÈÐ";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 4;
            doenchant = true;
            dobless = false;
            need_item = false;
            item_relation = 0;
            item1_id = -1;
            item1_num = 0;
            item2_id = -1;
            item2_num = 0;
            weapon_limit = -1;
            skill_limit = 0;
#ifdef _SKILL_SERVER
            statestub.push_back (new State1 ());
            statestub.push_back (new State2 ());
#endif
        }
        virtual ~ Skill2048Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 252 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (100 * skill->GetLevel () + 77 + skill->GetLevel () * skill->GetLevel ());
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 40000;
        }
        int GetEnmity (Skill * skill) const
        {
            return 1;
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetRadius (Skill * skill) const
        {
            return (float) (5);
        }
        float GetAngle (Skill * skill) const
        {
            return (float) (1 - 0.0111111 * (120));
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (5);
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (3);
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format,
                               77 + 100 * skill->GetLevel () + skill->GetLevel () * skill->GetLevel (),
                               8 + 2 * skill->GetLevel (), 50 + 5 * skill->GetLevel (), 2 * skill->GetLevel (), 5 + skill->GetLevel ());

        }
#endif
#ifdef _SKILL_CLIENT
        int GetIntroduction (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, skill->GetShowlevel ());

        }
#endif
#ifdef _SKILL_SERVER
        bool StateAttack (Skill * skill) const
        {
            skill->GetVictim ()->SetProbability (1.0 * (120));
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetBuffid (1);
            skill->GetVictim ()->SetAmount (10);
            skill->GetVictim ()->SetValue (0.02 * skill->GetLevel () * skill->GetPlayer ()->GetMaxhp ());
            skill->GetVictim ()->SetSubhp (1);
            skill->GetVictim ()->SetProbability (1.0 * (skill->GetPlayer ()->GetHp () * 2 < skill->GetPlayer ()->GetMaxhp ()? 120 : 0));
            skill->GetVictim ()->SetTime (5000 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetRatio (0);
            skill->GetVictim ()->SetBuffid (9);
            skill->GetVictim ()->SetFrozen (1);
            skill->GetVictim ()->SetProbability (1.0 * (-1));
            skill->GetVictim ()->SetTime (500 + 500 * skill->GetT0 ());
            skill->GetVictim ()->SetDiet (1);
            return true;
        }
        int GetCoverage (Skill * skill) const
        {
            return (int) (8 + 2 * skill->GetLevel ());
        }
#endif
    };
}
#endif
