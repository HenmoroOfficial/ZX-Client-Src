#ifndef __CPPGEN_GNET_SKILL2057
#define __CPPGEN_GNET_SKILL2057
namespace GNET
{
#ifdef _SKILL_SERVER
    class Skill2057:public Skill
    {
      public:
        enum
        { SKILL_ID = 2057 };
          Skill2057 ():Skill (SKILL_ID)
        {
        }
    };
#endif
    class Skill2057Stub:public SkillStub
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
                skill->GetPlayer ()->SetPerform (1);
            }
        };
#endif
      Skill2057Stub ():SkillStub (2057)
        {
            occupation = 149;
            name = L"¼¤ÀøII";
            nativename = "¼¤ÀøII";
            icon = "¼¤ÀøII.dds";
            maxlevel = 12;
            maxlearn = 10;
            type = 2;
            eventflag = 1;
            spcost = 0;
            timetype = 0;
            targettype = 0;
            autoattack = 0;
            allowform = 0;
            credittype = 10;
            clearmask = 130;
            serialskill = 0;
            charge = 0;
            chargedist = 0;
            succeedskillcolor = 0;
            succeedskillcolornum = 0;
            comboskill = 0;
            cycle = 0;
            cyclegfx = "";
            cyclemode = 0;
            skill_class = 1;
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
            action[0] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[1] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[2] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[3] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[4] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[5] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[6] = "ºÏ»¶ÅÉ_¼¤Àø";
            action[7] = "0";
            action[8] = "0";
            action[9] = "0";
            action[10] = "0";
            action[11] = "0";
            action[12] = "0";
            action[13] = "";
            action[14] = "";
            action[15] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[16] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[17] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[18] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[19] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[20] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[21] = "ºÏ»¶ÅÉ_Æï³Ë_¼¤Àø";
            action[22] = "0";
            action[23] = "0";
            action[24] = "0";
            action[25] = "0";
            action[26] = "0";
            action[27] = "0";
            action[28] = "";
            action[29] = "";
            action[30] = "0";
            rangetype = 5;
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
        virtual ~ Skill2057Stub ()
        {
        }
        int GetDpcost (Skill * skill) const
        {
            return (int) (0);
        }
        int GetPreSkillID (int index) const
        {
            static int array[1] = { 299 };
            return array[index];
        }
        int GetPreSkillSP (int index) const
        {
            static int array[1] = { 9 };
            return array[index];
        }
        float GetMpcost (Skill * skill) const
        {
            return (float) (1900);
        }
        int GetExecutetime (Skill * skill) const
        {
            return 1000;
        }
        int GetCooldowntime (Skill * skill) const
        {
            return 380000 - 20000 * skill->GetLevel ();
        }
        int GetRequiredLevel (int level) const
        {
            static int array[10] = { 125, 125, 125, 125, 125, 127, 129, 131, 133, 135 };
            return array[level - 1];
        }
        float GetPraydistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        float GetCastdistance (Skill * skill) const
        {
            return (float) (skill->GetPlayer ()->GetRange ());
        }
        int GetCreditCost (int level) const
        {
            static int array[10] = { 18000, 20000, 22000, 24000, 26000, 28000, 30000, 32000, 38000, 45000 };
            return array[level - 1];
        }
#ifdef _SKILL_CLIENT
        int GetDescription (Skill * skill, wchar_t * buffer, int length, wchar_t * format, const SkillStr & table) const
        {
            return _snwprintf (buffer, length, format, 1900, 5 + skill->GetLevel (), 8 + 2 * skill->GetLevel (), 380 - 20 * skill->GetLevel ());

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
            skill->GetVictim ()->SetClearwrap (1);
            skill->GetVictim ()->SetTime ((8000 + 2000 * skill->GetLevel ()) + 100);
            skill->GetVictim ()->SetImmunewrap (1);
            skill->GetVictim ()->SetTime (5100 + 1000 * skill->GetLevel ());
            skill->GetVictim ()->SetMakecrit (1);
            return true;
        }
#endif
    };
}
#endif
